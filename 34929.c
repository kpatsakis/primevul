jbig2_table_free(Jbig2Ctx *ctx, Jbig2HuffmanParams *params)
{
    if (params != NULL) {
        if (params->lines != NULL)
            jbig2_free(ctx->allocator, (void *)params->lines);
        jbig2_free(ctx->allocator, params);
    }
}
