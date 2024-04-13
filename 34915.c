jbig2_decode_pattern_dict(Jbig2Ctx *ctx, Jbig2Segment *segment,
                          const Jbig2PatternDictParams *params, const byte *data, const size_t size, Jbig2ArithCx *GB_stats)
{
    Jbig2PatternDict *hd = NULL;
    Jbig2Image *image = NULL;
    Jbig2GenericRegionParams rparams;
    int code = 0;

    /* allocate the collective image */
    image = jbig2_image_new(ctx, params->HDPW * (params->GRAYMAX + 1), params->HDPH);
    if (image == NULL) {
        jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "failed to allocate collective bitmap for halftone dict!");
        return NULL;
    }

    /* fill out the generic region decoder parameters */
    rparams.MMR = params->HDMMR;
    rparams.GBTEMPLATE = params->HDTEMPLATE;
    rparams.TPGDON = 0;         /* not used if HDMMR = 1 */
    rparams.USESKIP = 0;
    rparams.gbat[0] = -(int8_t) params->HDPW;
    rparams.gbat[1] = 0;
    rparams.gbat[2] = -3;
    rparams.gbat[3] = -1;
    rparams.gbat[4] = 2;
    rparams.gbat[5] = -2;
    rparams.gbat[6] = -2;
    rparams.gbat[7] = -2;

    if (params->HDMMR) {
        code = jbig2_decode_generic_mmr(ctx, segment, &rparams, data, size, image);
    } else {
        Jbig2WordStream *ws = jbig2_word_stream_buf_new(ctx, data, size);

        if (ws != NULL) {
            Jbig2ArithState *as = jbig2_arith_new(ctx, ws);

            if (as != NULL) {
                code = jbig2_decode_generic_region(ctx, segment, &rparams, as, image, GB_stats);
            } else {
                code = jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "failed to allocate storage for as in halftone dict!");
            }

            jbig2_free(ctx->allocator, as);
            jbig2_word_stream_buf_free(ctx, ws);
        } else {
            code = jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "failed to allocate storage for ws in halftone dict!");
        }
    }

    if (code == 0)
        hd = jbig2_hd_new(ctx, params, image);
    jbig2_image_release(ctx, image);

    return hd;
}
