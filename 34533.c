int ff_h263_decode_mba(MpegEncContext *s)
{
    int i, mb_pos;

    for (i = 0; i < 6; i++)
        if (s->mb_num - 1 <= ff_mba_max[i])
            break;
    mb_pos  = get_bits(&s->gb, ff_mba_length[i]);
    s->mb_x = mb_pos % s->mb_width;
    s->mb_y = mb_pos / s->mb_width;

    return mb_pos;
}
