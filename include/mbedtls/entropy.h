/*
 * Thin compatibility header for projects that still include legacy
 * `mbedtls/entropy.h` on ESP-IDF v6+ (mbedTLS v4, PSA-based).
 *
 * Scope: provide minimal types/functions needed by TLS call-sites that used
 * entropy+CTR_DRBG. This does NOT restore removed TLS APIs (e.g.
 * `mbedtls_ssl_conf_rng()`), it only prevents include/build breaks.
 */
#pragma once

#include <stddef.h>

#include "psa/crypto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mbedtls_entropy_context {
    unsigned char _unused;
} mbedtls_entropy_context;

static inline void mbedtls_entropy_init(mbedtls_entropy_context *ctx)
{
    (void) ctx;
}

static inline void mbedtls_entropy_free(mbedtls_entropy_context *ctx)
{
    (void) ctx;
}

static inline int mbedtls_entropy_func(void *data, unsigned char *output, size_t len)
{
    (void) data;
    (void) psa_crypto_init(); // idempotent
    return psa_generate_random(output, len) == PSA_SUCCESS ? 0 : -1;
}

#ifdef __cplusplus
}
#endif
