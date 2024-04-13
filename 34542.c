static int h263_skip_b_part(MpegEncContext *s, int cbp)
{
    LOCAL_ALIGNED_16(int16_t, dblock, [64]);
    int i, mbi;

    /* we have to set s->mb_intra to zero to decode B-part of PB-frame correctly
     * but real value should be restored in order to be used later (in OBMC condition)
     */
    mbi = s->mb_intra;
    s->mb_intra = 0;
    for (i = 0; i < 6; i++) {
        if (h263_decode_block(s, dblock, i, cbp&32) < 0)
            return -1;
        cbp+=cbp;
    }
    s->mb_intra = mbi;
    return 0;
}
