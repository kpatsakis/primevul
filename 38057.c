static av_cold void jpg_free_context(JPGContext *ctx)
{
    int i;

    for (i = 0; i < 2; i++) {
        ff_free_vlc(&ctx->dc_vlc[i]);
        ff_free_vlc(&ctx->ac_vlc[i]);
    }

    av_freep(&ctx->buf);
}
