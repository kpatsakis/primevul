jbig2_hd_new(Jbig2Ctx *ctx, const Jbig2PatternDictParams *params, Jbig2Image *image)
{
    Jbig2PatternDict *new;
    const int N = params->GRAYMAX + 1;
    const int HPW = params->HDPW;
    const int HPH = params->HDPH;
    int i;

    /* allocate a new struct */
    new = jbig2_new(ctx, Jbig2PatternDict, 1);
    if (new != NULL) {
        new->patterns = jbig2_new(ctx, Jbig2Image *, N);
        if (new->patterns == NULL) {
            jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "failed to allocate pattern in collective bitmap dictionary");
            jbig2_free(ctx->allocator, new);
            return NULL;
        }
        new->n_patterns = N;
        new->HPW = HPW;
        new->HPH = HPH;

        /* 6.7.5(4) - copy out the individual pattern images */
        for (i = 0; i < N; i++) {
            new->patterns[i] = jbig2_image_new(ctx, HPW, HPH);
            if (new->patterns[i] == NULL) {
                int j;

                jbig2_error(ctx, JBIG2_SEVERITY_WARNING, -1, "failed to allocate pattern element image");
                for (j = 0; j < i; j++)
                    jbig2_free(ctx->allocator, new->patterns[j]);
                jbig2_free(ctx->allocator, new);
                return NULL;
            }
            /* compose with the REPLACE operator; the source
               will be clipped to the destintion, selecting the
               proper sub image */
            jbig2_image_compose(ctx, new->patterns[i], image, -i * HPW, 0, JBIG2_COMPOSE_REPLACE);
        }
    } else {
        jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "failed to allocate collective bitmap dictionary");
    }

    return new;
}
