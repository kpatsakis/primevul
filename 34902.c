jbig2_decode_generic_region(Jbig2Ctx *ctx,
                            Jbig2Segment *segment, const Jbig2GenericRegionParams *params, Jbig2ArithState *as, Jbig2Image *image, Jbig2ArithCx *GB_stats)
{
    const int8_t *gbat = params->gbat;

    if (image->stride * image->height > (1 << 24) && segment->data_length < image->stride * image->height / 256) {
        return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number,
                           "region is far larger than data provided (%d << %d), aborting to prevent DOS", segment->data_length, image->stride * image->height);
    }

    if (!params->MMR && params->TPGDON)
        return jbig2_decode_generic_region_TPGDON(ctx, segment, params, as, image, GB_stats);

    if (!params->MMR && params->GBTEMPLATE == 0) {
        if (gbat[0] == +3 && gbat[1] == -1 && gbat[2] == -3 && gbat[3] == -1 && gbat[4] == +2 && gbat[5] == -2 && gbat[6] == -2 && gbat[7] == -2)
            return jbig2_decode_generic_template0(ctx, segment, params, as, image, GB_stats);
        else
            return jbig2_decode_generic_template0_unopt(ctx, segment, params, as, image, GB_stats);
    } else if (!params->MMR && params->GBTEMPLATE == 1)
        return jbig2_decode_generic_template1(ctx, segment, params, as, image, GB_stats);
    else if (!params->MMR && params->GBTEMPLATE == 2) {
        if (gbat[0] == 3 && gbat[1] == -1)
            return jbig2_decode_generic_template2a(ctx, segment, params, as, image, GB_stats);
        else
            return jbig2_decode_generic_template2(ctx, segment, params, as, image, GB_stats);
    } else if (!params->MMR && params->GBTEMPLATE == 3) {
        if (gbat[0] == 2 && gbat[1] == -1)
            return jbig2_decode_generic_template3_unopt(ctx, segment, params, as, image, GB_stats);
        else
            return jbig2_decode_generic_template3_unopt(ctx, segment, params, as, image, GB_stats);
    }

    {
        int i;

        for (i = 0; i < 8; i++)
                    jbig2_error(ctx, JBIG2_SEVERITY_DEBUG, segment->number, "gbat[%d] = %d", i, params->gbat[i]);
    }
    jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "decode_generic_region: MMR=%d, GBTEMPLATE=%d NYI", params->MMR, params->GBTEMPLATE);
    return -1;
}
