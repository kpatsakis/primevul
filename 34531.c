av_cold void ff_h263_decode_init_vlc(void)
{
    static int done = 0;

    if (!done) {
        done = 1;

        INIT_VLC_STATIC(&ff_h263_intra_MCBPC_vlc, INTRA_MCBPC_VLC_BITS, 9,
                 ff_h263_intra_MCBPC_bits, 1, 1,
                 ff_h263_intra_MCBPC_code, 1, 1, 72);
        INIT_VLC_STATIC(&ff_h263_inter_MCBPC_vlc, INTER_MCBPC_VLC_BITS, 28,
                 ff_h263_inter_MCBPC_bits, 1, 1,
                 ff_h263_inter_MCBPC_code, 1, 1, 198);
        INIT_VLC_STATIC(&ff_h263_cbpy_vlc, CBPY_VLC_BITS, 16,
                 &ff_h263_cbpy_tab[0][1], 2, 1,
                 &ff_h263_cbpy_tab[0][0], 2, 1, 64);
        INIT_VLC_STATIC(&mv_vlc, MV_VLC_BITS, 33,
                 &ff_mvtab[0][1], 2, 1,
                 &ff_mvtab[0][0], 2, 1, 538);
        ff_rl_init(&ff_h263_rl_inter, ff_h263_static_rl_table_store[0]);
        ff_rl_init(&ff_rl_intra_aic, ff_h263_static_rl_table_store[1]);
        INIT_VLC_RL(ff_h263_rl_inter, 554);
        INIT_VLC_RL(ff_rl_intra_aic, 554);
        INIT_VLC_STATIC(&h263_mbtype_b_vlc, H263_MBTYPE_B_VLC_BITS, 15,
                 &ff_h263_mbtype_b_tab[0][1], 2, 1,
                 &ff_h263_mbtype_b_tab[0][0], 2, 1, 80);
        INIT_VLC_STATIC(&cbpc_b_vlc, CBPC_B_VLC_BITS, 4,
                 &ff_cbpc_b_tab[0][1], 2, 1,
                 &ff_cbpc_b_tab[0][0], 2, 1, 8);
    }
}
