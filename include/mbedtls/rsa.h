/*
 * Compatibility header to expose mbedTLS v4 private RSA APIs.
 *
 * mbedTLS v4 moved RSA types and functions entirely to private headers.
 * There is no public mbedtls/rsa.h in v4, so this header provides the
 * interface by including the private RSA header directly.
 */
#pragma once

#ifndef MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#define MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#endif

#include "mbedtls/build_info.h"

/* In mbedTLS v4, there is no public rsa.h - RSA APIs are entirely private.
 * Include the private RSA header directly for mbedtls_rsa_context and RSA functions. */
#include "mbedtls/private/rsa.h"
