static av_cold int dpcm_decode_init(AVCodecContext *avctx)
{
    DPCMContext *s = avctx->priv_data;
    int i;

    if (avctx->channels < 1 || avctx->channels > 2) {
        av_log(avctx, AV_LOG_INFO, "invalid number of channels\n");
        return AVERROR(EINVAL);
    }

    s->channels = avctx->channels;
    s->sample[0] = s->sample[1] = 0;

    switch(avctx->codec->id) {

    case CODEC_ID_ROQ_DPCM:
        /* initialize square table */
        for (i = 0; i < 128; i++) {
            int16_t square = i * i;
            s->roq_square_array[i      ] =  square;
            s->roq_square_array[i + 128] = -square;
        }
        break;

    case CODEC_ID_SOL_DPCM:
        switch(avctx->codec_tag){
        case 1:
            s->sol_table = sol_table_old;
            s->sample[0] = s->sample[1] = 0x80;
            break;
        case 2:
            s->sol_table = sol_table_new;
            s->sample[0] = s->sample[1] = 0x80;
            break;
        case 3:
            break;
        default:
            av_log(avctx, AV_LOG_ERROR, "Unknown SOL subcodec\n");
            return -1;
        }
        break;

    default:
        break;
    }

    if (avctx->codec->id == CODEC_ID_SOL_DPCM && avctx->codec_tag != 3)
        avctx->sample_fmt = AV_SAMPLE_FMT_U8;
    else
        avctx->sample_fmt = AV_SAMPLE_FMT_S16;

    avcodec_get_frame_defaults(&s->frame);
    avctx->coded_frame = &s->frame;

    return 0;
}
