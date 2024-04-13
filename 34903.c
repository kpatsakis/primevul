jbig2_decode_generic_region_TPGDON(Jbig2Ctx *ctx,
                                   Jbig2Segment *segment,
                                   const Jbig2GenericRegionParams *params, Jbig2ArithState *as, Jbig2Image *image, Jbig2ArithCx *GB_stats)
{
    switch (params->GBTEMPLATE) {
    case 0:
        return jbig2_decode_generic_template0_TPGDON(ctx, segment, params, as, image, GB_stats);
    case 1:
        return jbig2_decode_generic_template1_TPGDON(ctx, segment, params, as, image, GB_stats);
    case 2:
        return jbig2_decode_generic_template2_TPGDON(ctx, segment, params, as, image, GB_stats);
    case 3:
        return jbig2_decode_generic_template3_TPGDON(ctx, segment, params, as, image, GB_stats);
    }

    return -1;
}
