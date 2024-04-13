static void preview_obmc(MpegEncContext *s){
    GetBitContext gb= s->gb;

    int cbpc, i, pred_x, pred_y, mx, my;
    int16_t *mot_val;
    const int xy= s->mb_x + 1 + s->mb_y * s->mb_stride;
    const int stride= s->b8_stride*2;

    for(i=0; i<4; i++)
        s->block_index[i]+= 2;
    for(i=4; i<6; i++)
        s->block_index[i]+= 1;
    s->mb_x++;

    assert(s->pict_type == AV_PICTURE_TYPE_P);

    do{
        if (get_bits1(&s->gb)) {
            /* skip mb */
            mot_val = s->current_picture.motion_val[0][s->block_index[0]];
            mot_val[0       ]= mot_val[2       ]=
            mot_val[0+stride]= mot_val[2+stride]= 0;
            mot_val[1       ]= mot_val[3       ]=
            mot_val[1+stride]= mot_val[3+stride]= 0;

            s->current_picture.mb_type[xy] = MB_TYPE_SKIP | MB_TYPE_16x16 | MB_TYPE_L0;
            goto end;
        }
        cbpc = get_vlc2(&s->gb, ff_h263_inter_MCBPC_vlc.table, INTER_MCBPC_VLC_BITS, 2);
    }while(cbpc == 20);

    if(cbpc & 4){
        s->current_picture.mb_type[xy] = MB_TYPE_INTRA;
    }else{
        get_vlc2(&s->gb, ff_h263_cbpy_vlc.table, CBPY_VLC_BITS, 1);
        if (cbpc & 8) {
            if(s->modified_quant){
                if(get_bits1(&s->gb)) skip_bits(&s->gb, 1);
                else                  skip_bits(&s->gb, 5);
            }else
                skip_bits(&s->gb, 2);
        }

        if ((cbpc & 16) == 0) {
                s->current_picture.mb_type[xy] = MB_TYPE_16x16 | MB_TYPE_L0;
                /* 16x16 motion prediction */
                mot_val= ff_h263_pred_motion(s, 0, 0, &pred_x, &pred_y);
                if (s->umvplus)
                   mx = h263p_decode_umotion(s, pred_x);
                else
                   mx = ff_h263_decode_motion(s, pred_x, 1);

                if (s->umvplus)
                   my = h263p_decode_umotion(s, pred_y);
                else
                   my = ff_h263_decode_motion(s, pred_y, 1);

                mot_val[0       ]= mot_val[2       ]=
                mot_val[0+stride]= mot_val[2+stride]= mx;
                mot_val[1       ]= mot_val[3       ]=
                mot_val[1+stride]= mot_val[3+stride]= my;
        } else {
            s->current_picture.mb_type[xy] = MB_TYPE_8x8 | MB_TYPE_L0;
            for(i=0;i<4;i++) {
                mot_val = ff_h263_pred_motion(s, i, 0, &pred_x, &pred_y);
                if (s->umvplus)
                  mx = h263p_decode_umotion(s, pred_x);
                else
                  mx = ff_h263_decode_motion(s, pred_x, 1);

                if (s->umvplus)
                  my = h263p_decode_umotion(s, pred_y);
                else
                  my = ff_h263_decode_motion(s, pred_y, 1);
                if (s->umvplus && (mx - pred_x) == 1 && (my - pred_y) == 1)
                  skip_bits1(&s->gb); /* Bit stuffing to prevent PSC */
                mot_val[0] = mx;
                mot_val[1] = my;
            }
        }
    }
end:

    for(i=0; i<4; i++)
        s->block_index[i]-= 2;
    for(i=4; i<6; i++)
        s->block_index[i]-= 1;
    s->mb_x--;

    s->gb= gb;
}
