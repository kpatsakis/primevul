static int dv_extract_video_info(DVDemuxContext *c, uint8_t* frame)
{
    const uint8_t* vsc_pack;
    AVCodecContext* avctx;
    int apt, is16_9;
    int size = 0;

    if (c->sys) {
        avctx = c->vst->codec;

        avpriv_set_pts_info(c->vst, 64, c->sys->time_base.num,
                        c->sys->time_base.den);
        avctx->time_base= c->sys->time_base;
        if (!avctx->width){
            avctx->width = c->sys->width;
            avctx->height = c->sys->height;
        }
        avctx->pix_fmt = c->sys->pix_fmt;

        /* finding out SAR is a little bit messy */
        vsc_pack = dv_extract_pack(frame, dv_video_control);
        apt      = frame[4] & 0x07;
        is16_9   = (vsc_pack && ((vsc_pack[2] & 0x07) == 0x02 ||
                                (!apt && (vsc_pack[2] & 0x07) == 0x07)));
        c->vst->sample_aspect_ratio = c->sys->sar[is16_9];
        avctx->bit_rate = av_rescale_q(c->sys->frame_size, (AVRational){8,1},
                                       c->sys->time_base);
        size = c->sys->frame_size;
    }
    return size;
}
