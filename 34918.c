jbig2_pattern_dictionary(Jbig2Ctx *ctx, Jbig2Segment *segment, const byte *segment_data)
{
    Jbig2PatternDictParams params;
    Jbig2ArithCx *GB_stats = NULL;
    byte flags;
    int offset = 0;

    /* 7.4.4.1 - Data header */
    if (segment->data_length < 7) {
        return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "Segment too short");
    }
    flags = segment_data[0];
    params.HDMMR = flags & 1;
    params.HDTEMPLATE = (flags & 6) >> 1;
    params.HDPW = segment_data[1];
    params.HDPH = segment_data[2];
    params.GRAYMAX = jbig2_get_uint32(segment_data + 3);
    offset += 7;

    jbig2_error(ctx, JBIG2_SEVERITY_INFO, segment->number,
                "pattern dictionary, flags=%02x, %d grays (%dx%d cell)", flags, params.GRAYMAX + 1, params.HDPW, params.HDPH);

    if (params.HDMMR && params.HDTEMPLATE) {
        jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "HDTEMPLATE is %d when HDMMR is %d, contrary to spec", params.HDTEMPLATE, params.HDMMR);
    }
    if (flags & 0xf8) {
        jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "Reserved flag bits non-zero");
    }

    /* 7.4.4.2 */
    if (!params.HDMMR) {
        /* allocate and zero arithmetic coding stats */
        int stats_size = jbig2_generic_stats_size(ctx, params.HDTEMPLATE);

        GB_stats = jbig2_new(ctx, Jbig2ArithCx, stats_size);
        if (GB_stats == NULL) {
            jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "failed to allocate GB_stats in pattern dictionary");
            return 0;
        }
        memset(GB_stats, 0, stats_size);
    }

    segment->result = jbig2_decode_pattern_dict(ctx, segment, &params, segment_data + offset, segment->data_length - offset, GB_stats);

    /* todo: retain GB_stats? */
    if (!params.HDMMR) {
        jbig2_free(ctx->allocator, GB_stats);
    }

    return (segment->result != NULL) ? 0 : -1;
}
