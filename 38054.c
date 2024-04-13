static av_cold int g2m_decode_init(AVCodecContext *avctx)
{
    G2MContext * const c = avctx->priv_data;
    int ret;

    if ((ret = jpg_init(avctx, &c->jc)) != 0) {
        av_log(avctx, AV_LOG_ERROR, "Cannot initialise VLCs\n");
        jpg_free_context(&c->jc);
        return AVERROR(ENOMEM);
    }

    avctx->pix_fmt = AV_PIX_FMT_RGB24;

    return 0;
}
