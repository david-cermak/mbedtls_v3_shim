/*
 * Thin compatibility header for projects that still include legacy
 * `mbedtls/ctr_drbg.h` on ESP-IDF v6+ (mbedTLS v4, PSA-based).
 *
 * Scope: minimal typedefs + inlines so old call-sites compile.
 * RNG output is backed by PSA (`psa_generate_random`).
 */
#pragma once

#include <stddef.h>

#include "mbedtls/entropy.h"
#include "mbedtls/ssl_compat.h"
#include "mbedtls/pk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Legacy prediction resistance defines - PSA RNG handles this internally */
#define MBEDTLS_CTR_DRBG_PR_OFF 0
#define MBEDTLS_CTR_DRBG_PR_ON  1

typedef struct mbedtls_ctr_drbg_context {
    unsigned char _unused;
} mbedtls_ctr_drbg_context;

static inline void mbedtls_ctr_drbg_init(mbedtls_ctr_drbg_context *ctx)
{
    (void) ctx;
}

static inline void mbedtls_ctr_drbg_free(mbedtls_ctr_drbg_context *ctx)
{
    (void) ctx;
}

static inline int mbedtls_ctr_drbg_seed(mbedtls_ctr_drbg_context *ctx,
                                        int (*f_entropy)(void *, unsigned char *, size_t),
                                        void *p_entropy,
                                        const unsigned char *custom,
                                        size_t len)
{
    (void) ctx;
    (void) f_entropy;
    (void) p_entropy;
    (void) custom;
    (void) len;
    // PSA RNG does not require explicit seeding here.
    return 0;
}

static inline int mbedtls_ctr_drbg_random(void *p_rng, unsigned char *output, size_t output_len)
{
    (void) p_rng;
    return mbedtls_entropy_func(NULL, output, output_len);
}

/*
 * mbedtls_ctr_drbg_set_prediction_resistance() was removed in mbedTLS v4.
 * PSA RNG handles prediction resistance internally.
 * This is a no-op compatibility wrapper.
 */
static inline void mbedtls_ctr_drbg_set_prediction_resistance(
    mbedtls_ctr_drbg_context *ctx, int resistance)
{
    (void) ctx;
    (void) resistance;
}

/*
 * mbedtls_ctr_drbg_reseed() was removed in mbedTLS v4.
 * PSA RNG handles reseeding internally.
 * This is a no-op compatibility wrapper.
 */
static inline int mbedtls_ctr_drbg_reseed(mbedtls_ctr_drbg_context *ctx,
                                          const unsigned char *additional,
                                          size_t len)
{
    (void) ctx;
    (void) additional;
    (void) len;
    return 0;
}

#ifdef __cplusplus
}
#endif
