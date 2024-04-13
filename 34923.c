jbig2_huffman_free(Jbig2Ctx *ctx, Jbig2HuffmanState *hs)
{
    if (hs != NULL)
        jbig2_free(ctx->allocator, hs);
    return;
}
