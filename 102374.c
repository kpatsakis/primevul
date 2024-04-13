void bta_av_rc_remote_cmd(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RCB* p_rcb;
 if (p_cb->features & BTA_AV_FEAT_RCCT) {
 if (p_data->hdr.layer_specific < BTA_AV_NUM_RCB) {
      p_rcb = &p_cb->rcb[p_data->hdr.layer_specific];
 if (p_rcb->status & BTA_AV_RC_CONN_MASK) {
        AVRC_PassCmd(p_rcb->handle, p_data->api_remote_cmd.label,
 &p_data->api_remote_cmd.msg);
 }
 }
 }
}
