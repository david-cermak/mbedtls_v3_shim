/*
 * Compatibility header to expose mbedTLS v4 private ECDH APIs.
 *
 * mbedTLS v4 moved ECDH types and functions entirely to private headers.
 * There is no public mbedtls/ecdh.h in v4, so this header provides the
 * interface by including the private ECDH header directly.
 */
#pragma once

#ifndef MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#define MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#endif

#include "mbedtls/build_info.h"

/* In mbedTLS v4, there is no public ecdh.h - ECDH APIs are entirely private.
 * Include the private ECDH header directly for mbedtls_ecdh_context and ECDH functions. */
#include "mbedtls/private/ecdh.h"
