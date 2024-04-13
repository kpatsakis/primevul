jbig2_free_segment(Jbig2Ctx *ctx, Jbig2Segment *segment)
{
    if (segment->referred_to_segments != NULL) {
        jbig2_free(ctx->allocator, segment->referred_to_segments);
    }
    /* todo: we need either some separate fields or
       a more complex result object rather than this
       brittle special casing */
    switch (segment->flags & 63) {
    case 0:                    /* symbol dictionary */
        if (segment->result != NULL)
            jbig2_sd_release(ctx, (Jbig2SymbolDict *) segment->result);
        break;
    case 4:                    /* intermediate text region */
    case 40:                   /* intermediate refinement region */
        if (segment->result != NULL)
            jbig2_image_release(ctx, (Jbig2Image *) segment->result);
        break;
    case 16:                   /* pattern dictionary */
        if (segment->result != NULL)
            jbig2_hd_release(ctx, (Jbig2PatternDict *) segment->result);
        break;
    case 53:                   /* user-supplied huffman table */
        if (segment->result != NULL)
            jbig2_table_free(ctx, (Jbig2HuffmanParams *) segment->result);
        break;
    case 62:
        if (segment->result != NULL)
            jbig2_metadata_free(ctx, (Jbig2Metadata *) segment->result);
        break;
    default:
        /* anything else is probably an undefined pointer */
        break;
    }
    jbig2_free(ctx->allocator, segment);
}
