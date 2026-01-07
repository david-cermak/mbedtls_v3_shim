/*
 * Thin compatibility header providing APIs removed in mbedTLS v4.
 */
#pragma once

#include "mbedtls/ssl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * mbedtls_ssl_conf_rng() was removed in mbedTLS v4.
 * TLS now uses the PSA random generator internally.
 * This is a no-op compatibility wrapper.
 */
static inline void mbedtls_ssl_conf_rng(mbedtls_ssl_config *conf,
                                        int (*f_rng)(void *, unsigned char *, size_t),
                                        void *p_rng)
{
    (void) conf;
    (void) f_rng;
    (void) p_rng;
}

#ifdef __cplusplus
}
#endif
