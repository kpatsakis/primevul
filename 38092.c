static av_cold int rpza_decode_init(AVCodecContext *avctx)
{
    RpzaContext *s = avctx->priv_data;

    s->avctx = avctx;
    avctx->pix_fmt = AV_PIX_FMT_RGB555;

    avcodec_get_frame_defaults(&s->frame);

    return 0;
}
