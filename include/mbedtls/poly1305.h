/*
 * Compatibility header for mbedTLS v4 which removed public poly1305.h.
 *
 * mbedTLS v4 moved poly1305 to private headers and recommends using PSA API.
 * This header provides minimal declarations to allow legacy code to compile.
 */
#pragma once

#ifndef MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#define MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#endif

#include "mbedtls/build_info.h"

/* Try to include private poly1305 header if available */
#include "mbedtls/private/poly1305.h"
