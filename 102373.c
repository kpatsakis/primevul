static void bta_av_close_all_rc(tBTA_AV_CB* p_cb) {
 int i;

 for (i = 0; i < BTA_AV_NUM_RCB; i++) {
 if ((p_cb->disabling) || (bta_av_cb.rcb[i].shdl != 0))
      bta_av_del_rc(&bta_av_cb.rcb[i]);
 }
}
