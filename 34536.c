int ff_h263_resync(MpegEncContext *s){
    int left, pos, ret;

    if(s->codec_id==AV_CODEC_ID_MPEG4){
        skip_bits1(&s->gb);
        align_get_bits(&s->gb);
    }

    if(show_bits(&s->gb, 16)==0){
        pos= get_bits_count(&s->gb);
        if(CONFIG_MPEG4_DECODER && s->codec_id==AV_CODEC_ID_MPEG4)
            ret= ff_mpeg4_decode_video_packet_header(s->avctx->priv_data);
        else
            ret= h263_decode_gob_header(s);
        if(ret>=0)
            return pos;
    }
    s->gb= s->last_resync_gb;
    align_get_bits(&s->gb);
    left= get_bits_left(&s->gb);

    for(;left>16+1+5+5; left-=8){
        if(show_bits(&s->gb, 16)==0){
            GetBitContext bak= s->gb;

            pos= get_bits_count(&s->gb);
            if(CONFIG_MPEG4_DECODER && s->codec_id==AV_CODEC_ID_MPEG4)
                ret= ff_mpeg4_decode_video_packet_header(s->avctx->priv_data);
            else
                ret= h263_decode_gob_header(s);
            if(ret>=0)
                return pos;

            s->gb= bak;
        }
        skip_bits(&s->gb, 8);
    }

    return -1;
}
