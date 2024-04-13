static av_cold int g2m_decode_end(AVCodecContext *avctx)
{
    G2MContext * const c = avctx->priv_data;

    jpg_free_context(&c->jc);

    av_freep(&c->kempf_buf);
    av_freep(&c->kempf_flags);
    av_freep(&c->synth_tile);
    av_freep(&c->jpeg_tile);
    av_freep(&c->cursor);
    av_freep(&c->framebuf);

    return 0;
}
