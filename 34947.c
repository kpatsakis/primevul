jbig2_parse_extension_segment(Jbig2Ctx *ctx, Jbig2Segment *segment, const uint8_t *segment_data)
{
    uint32_t type = jbig2_get_uint32(segment_data);
    bool reserved = type & 0x20000000;

    /* bool dependent = type & 0x40000000; (NYI) */
    bool necessary = type & 0x80000000;

    if (necessary && !reserved) {
        jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "extension segment is marked 'necessary' but " "not 'reservered' contrary to spec");
    }

    switch (type) {
    case 0x20000000:
        return jbig2_comment_ascii(ctx, segment, segment_data);
    case 0x20000002:
        return jbig2_comment_unicode(ctx, segment, segment_data);
    default:
        if (necessary) {
            return jbig2_error(ctx, JBIG2_SEVERITY_FATAL, segment->number, "unhandled necessary extension segment type 0x%08x", type);
        } else {
            return jbig2_error(ctx, JBIG2_SEVERITY_WARNING, segment->number, "unhandled extension segment");
        }
    }
}
