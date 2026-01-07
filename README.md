# mbedtls_v3_shim

A header-only ESP-IDF component that allows legacy code written for mbedTLS v3 to compile against mbedTLS v4 (PSA-based API).

## Background

ESP-IDF v6+ ships with mbedTLS v4, which introduces significant API changes:

1. **PSA Crypto API** - Random number generation and many cryptographic operations now use PSA internally
2. **Private headers** - Many types and functions moved from public headers to `mbedtls/private/*.h`
3. **Removed APIs** - Functions like `mbedtls_ssl_conf_rng()`, `mbedtls_ctr_drbg_reseed()`, and 3DES support were removed
4. **Changed signatures** - Functions like `mbedtls_pk_parse_key()` and `mbedtls_pk_sign()` no longer take RNG parameters

This component bridges the gap so that libraries like **libssh** (which use the legacy mbedTLS API) can compile without source modifications.

## How It Works

### 1. Private Header Exposure

mbedTLS v4 provides a mechanism to access private APIs via the `MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS` macro. When defined before including mbedTLS headers, it exposes internal types and functions from `mbedtls/private/*.h`.

Our compatibility headers define this macro and include the appropriate private headers:

```c
#ifndef MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#define MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#endif

#include "mbedtls/private/rsa.h"      // For mbedtls_rsa_context, mbedtls_rsa_*()
#include "mbedtls/private/ecdsa.h"    // For mbedtls_ecdsa_context, mbedtls_ecdsa_*()
#include "mbedtls/private/pk_private.h"  // For mbedtls_pk_type_t, MBEDTLS_PK_RSA, etc.
```

### 2. Header Chaining with `#include_next`

For headers that still exist publicly but need augmentation, we use GCC's `#include_next` directive to include the real header first, then add our compatibility extensions:

```c
#include_next "mbedtls/pk.h"           // Get the real public API
#include "mbedtls/private/pk_private.h" // Add private types
```

### 3. Signature Adaptation Macros

Where function signatures changed (RNG arguments removed), we provide inline wrappers and macros:

```c
// Wrapper calls the real v4 5-arg function
static inline int mbedtls_pk_parse_key_v4_real(
    mbedtls_pk_context *ctx,
    const unsigned char *key, size_t keylen,
    const unsigned char *pwd, size_t pwdlen)
{
    return mbedtls_pk_parse_key(ctx, key, keylen, pwd, pwdlen);
}

// Macro accepts legacy 7-arg calls, discards f_rng and p_rng
#define mbedtls_pk_parse_key(ctx, key, keylen, pwd, pwdlen, f_rng, p_rng) \
    mbedtls_pk_parse_key_v4_real(ctx, key, keylen, pwd, pwdlen)
```

### 4. PSA-Backed Stubs

For removed APIs like entropy and CTR_DRBG, we provide stub implementations backed by PSA:

```c
static inline int mbedtls_entropy_func(void *data, unsigned char *output, size_t len)
{
    (void) psa_crypto_init();  // Idempotent
    return psa_generate_random(output, len) == PSA_SUCCESS ? 0 : -1;
}
```

## Compatibility Headers

| Header | Purpose |
|--------|---------|
| `pk.h` | Exposes `mbedtls_pk_type_t`, `MBEDTLS_PK_RSA`, `MBEDTLS_PK_ECDSA`, etc. Wraps `mbedtls_pk_parse_key()` (7→5 args) and `mbedtls_pk_sign()` (9→7 args). Defines `MBEDTLS_PK_RSA_ALT` and `MBEDTLS_PK_ECKEY_DH`. |
| `rsa.h` | Exposes `mbedtls_rsa_context` and RSA functions (`mbedtls_rsa_init`, `mbedtls_rsa_gen_key`, `mbedtls_rsa_export`, etc.) |
| `ecdsa.h` | Exposes `mbedtls_ecdsa_context` and ECDSA functions (`mbedtls_ecdsa_init`, `mbedtls_ecdsa_sign`, `mbedtls_ecdsa_verify`, `mbedtls_ecdsa_genkey`, etc.) |
| `ecdh.h` | Exposes `mbedtls_ecdh_context` and ECDH functions |
| `entropy.h` | Provides `mbedtls_entropy_context` stub and `mbedtls_entropy_func()` backed by PSA |
| `ctr_drbg.h` | Provides `mbedtls_ctr_drbg_context` stub, `mbedtls_ctr_drbg_random()` via PSA, no-op `mbedtls_ctr_drbg_reseed()`, and `MBEDTLS_CTR_DRBG_PR_ON/OFF` defines |
| `ssl_compat.h` | No-op `mbedtls_ssl_conf_rng()` (TLS now uses PSA RNG internally) |
| `md.h` | Exposes HMAC functions (`mbedtls_md_hmac_starts`, `mbedtls_md_hmac_update`, `mbedtls_md_hmac_finish`) |
| `cipher.h` | Exposes cipher types; defines removed 3DES types (`MBEDTLS_CIPHER_DES_EDE3_CBC`, etc.) as `MBEDTLS_CIPHER_NONE` |
| `chacha20.h` | Exposes ChaCha20 from private headers |
| `poly1305.h` | Exposes Poly1305 from private headers |

## ESP-IDF Port Headers

ESP-IDF's mbedTLS port (`components/mbedtls/port/include/mbedtls/`) already provides some compatibility headers that this component leverages:

- **`bignum.h`** - Exposes `mbedtls_mpi` and bignum functions
- **`ecp.h`** - Exposes `mbedtls_ecp_keypair`, `mbedtls_ecp_group`, `mbedtls_ecp_point`, and ECP functions

These port headers already define `MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS` and include the private headers, so our compatibility layer works seamlessly with them.

## Usage

### In Your Component's CMakeLists.txt

Add `mbedtls_v3_shim` as a dependency:

```cmake
idf_component_register(
    SRCS "your_source.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES mbedtls mbedtls_v3_shim
)
```

### Include Order

The compatibility headers must be found **before** the standard mbedTLS headers. This is handled by placing this component's include directory first in the include path. For libssh, this was done in its `CMakeLists.txt`:

```cmake
idf_component_register(
    SRCS ${libssh_SRCS}
    INCLUDE_DIRS ${MBEDTLS_V3_SHIM_INCLUDE_DIR} ${LIBSSH_DIR}/include
    PRIV_REQUIRES mbedtls mbedtls_v3_shim
)
```

## Limitations

1. **3DES operations will fail at runtime** - The cipher types are defined but map to `MBEDTLS_CIPHER_NONE`
2. **No linker wrapping** - This is a header-only solution; if you need to intercept already-compiled code, linker wrapping would be required

## References

- [mbedTLS PSA Transition Guide](https://github.com/Mbed-TLS/mbedtls/blob/development/tf-psa-crypto/docs/psa-transition.md)
- [mbedTLS v4 ChangeLog](https://github.com/Mbed-TLS/mbedtls/blob/development/ChangeLog)
- [ESP-IDF mbedTLS Component](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/mbedtls.html)
