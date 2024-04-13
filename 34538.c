static int h263_decode_block(MpegEncContext * s, int16_t * block,
                             int n, int coded)
{
    int code, level, i, j, last, run;
    RLTable *rl = &ff_h263_rl_inter;
    const uint8_t *scan_table;
    GetBitContext gb= s->gb;

    scan_table = s->intra_scantable.permutated;
    if (s->h263_aic && s->mb_intra) {
        rl = &ff_rl_intra_aic;
        i = 0;
        if (s->ac_pred) {
            if (s->h263_aic_dir)
                scan_table = s->intra_v_scantable.permutated; /* left */
            else
                scan_table = s->intra_h_scantable.permutated; /* top */
        }
    } else if (s->mb_intra) {
        /* DC coef */
        if (CONFIG_RV10_DECODER && s->codec_id == AV_CODEC_ID_RV10) {
          if (s->rv10_version == 3 && s->pict_type == AV_PICTURE_TYPE_I) {
            int component, diff;
            component = (n <= 3 ? 0 : n - 4 + 1);
            level = s->last_dc[component];
            if (s->rv10_first_dc_coded[component]) {
                diff = ff_rv_decode_dc(s, n);
                if (diff == 0xffff)
                    return -1;
                level += diff;
                level = level & 0xff; /* handle wrap round */
                s->last_dc[component] = level;
            } else {
                s->rv10_first_dc_coded[component] = 1;
            }
          } else {
                level = get_bits(&s->gb, 8);
                if (level == 255)
                    level = 128;
          }
        }else{
            level = get_bits(&s->gb, 8);
            if((level&0x7F) == 0){
                av_log(s->avctx, AV_LOG_ERROR, "illegal dc %d at %d %d\n", level, s->mb_x, s->mb_y);
                if (s->avctx->err_recognition & AV_EF_BITSTREAM)
                    return -1;
            }
            if (level == 255)
                level = 128;
        }
        block[0] = level;
        i = 1;
    } else {
        i = 0;
    }
    if (!coded) {
        if (s->mb_intra && s->h263_aic)
            goto not_coded;
        s->block_last_index[n] = i - 1;
        return 0;
    }
retry:
    for(;;) {
        code = get_vlc2(&s->gb, rl->vlc.table, TEX_VLC_BITS, 2);
        if (code < 0){
            av_log(s->avctx, AV_LOG_ERROR, "illegal ac vlc code at %dx%d\n", s->mb_x, s->mb_y);
            return -1;
        }
        if (code == rl->n) {
            /* escape */
            if (CONFIG_FLV_DECODER && s->h263_flv > 1) {
                ff_flv2_decode_ac_esc(&s->gb, &level, &run, &last);
            } else {
                last = get_bits1(&s->gb);
                run = get_bits(&s->gb, 6);
                level = (int8_t)get_bits(&s->gb, 8);
                if(level == -128){
                    if (s->codec_id == AV_CODEC_ID_RV10) {
                        /* XXX: should patch encoder too */
                        level = get_sbits(&s->gb, 12);
                    }else{
                        level = get_bits(&s->gb, 5);
                        level |= get_sbits(&s->gb, 6)<<5;
                    }
                }
            }
        } else {
            run = rl->table_run[code];
            level = rl->table_level[code];
            last = code >= rl->last;
            if (get_bits1(&s->gb))
                level = -level;
        }
        i += run;
        if (i >= 64){
            if(s->alt_inter_vlc && rl == &ff_h263_rl_inter && !s->mb_intra){
                rl = &ff_rl_intra_aic;
                i = 0;
                s->gb= gb;
                s->bdsp.clear_block(block);
                goto retry;
            }
            av_log(s->avctx, AV_LOG_ERROR, "run overflow at %dx%d i:%d\n", s->mb_x, s->mb_y, s->mb_intra);
            return -1;
        }
        j = scan_table[i];
        block[j] = level;
        if (last)
            break;
        i++;
    }
not_coded:
    if (s->mb_intra && s->h263_aic) {
        ff_h263_pred_acdc(s, block, n);
        i = 63;
    }
    s->block_last_index[n] = i;
    return 0;
}
