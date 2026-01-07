/*
 * Compatibility wrapper header for mbedTLS v4 MD (message digest) APIs.
 *
 * In mbedTLS v4 the HMAC functions are available in the public md.h header
 * when MBEDTLS_MD_C is defined. This header ensures they are properly exposed.
 */
#pragma once

#ifndef MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#define MBEDTLS_DECLARE_PRIVATE_IDENTIFIERS
#endif

#include "mbedtls/build_info.h"

/* Include the real md.h to get HMAC functions */
#include_next "mbedtls/md.h"
