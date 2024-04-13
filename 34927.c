jbig2_release_huffman_table(Jbig2Ctx *ctx, Jbig2HuffmanTable *table)
{
    if (table != NULL) {
        jbig2_free(ctx->allocator, table->entries);
        jbig2_free(ctx->allocator, table);
    }
    return;
}
