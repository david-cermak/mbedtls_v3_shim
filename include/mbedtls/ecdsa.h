/*
 * Compatibility header to expose mbedTLS v4 private ECDSA APIs.
 *
 * mbedTLS v4 moved ECDSA types and functions entirely to private headers.
 * There is no public mbedtls/ecdsa.h in v4, so this header provides the
 * interface by including the private ECDSA header directly.
 */
#pragma once

#ifndef MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#define MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#endif

#include "mbedtls/build_info.h"

/* In mbedTLS v4, there is no public ecdsa.h - ECDSA APIs are entirely private.
 * Include the private ECDSA header directly for mbedtls_ecdsa_context and ECDSA functions. */
#include "mbedtls/private/ecdsa.h"
