/*
 * Compatibility header for mbedTLS v4 cipher API changes.
 *
 * mbedTLS v4 moved cipher.h to private headers and removed 3DES support.
 * This header provides compatibility by including the private cipher header
 * and adding legacy defines.
 */
#pragma once

#ifndef MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#define MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#endif

#include "mbedtls/build_info.h"

/* In mbedTLS v4, cipher.h is in private headers */
#include "mbedtls/private/cipher.h"

/*
 * 3DES (DES-EDE) was removed in mbedTLS v4.
 * Define the legacy cipher type to NONE to allow code to compile,
 * though 3DES operations will fail at runtime.
 */
#ifndef MBEDTLS_CIPHER_DES_EDE3_CBC
#define MBEDTLS_CIPHER_DES_EDE3_CBC MBEDTLS_CIPHER_NONE
#endif

#ifndef MBEDTLS_CIPHER_DES_EDE3_ECB
#define MBEDTLS_CIPHER_DES_EDE3_ECB MBEDTLS_CIPHER_NONE
#endif

#ifndef MBEDTLS_CIPHER_DES_CBC
#define MBEDTLS_CIPHER_DES_CBC MBEDTLS_CIPHER_NONE
#endif

#ifndef MBEDTLS_CIPHER_DES_ECB
#define MBEDTLS_CIPHER_DES_ECB MBEDTLS_CIPHER_NONE
#endif
