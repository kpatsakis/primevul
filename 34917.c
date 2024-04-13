jbig2_hd_release(Jbig2Ctx *ctx, Jbig2PatternDict *dict)
{
    int i;

    if (dict == NULL)
        return;
    for (i = 0; i < dict->n_patterns; i++)
        if (dict->patterns[i])
            jbig2_image_release(ctx, dict->patterns[i]);
    jbig2_free(ctx->allocator, dict->patterns);
    jbig2_free(ctx->allocator, dict);
}
