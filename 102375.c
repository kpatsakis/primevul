void bta_av_rc_vendor_rsp(tBTA_AV_CB* p_cb, tBTA_AV_DATA* p_data) {
  tBTA_AV_RCB* p_rcb;
 if ((p_cb->features & (BTA_AV_FEAT_RCTG | BTA_AV_FEAT_VENDOR)) ==
 (BTA_AV_FEAT_RCTG | BTA_AV_FEAT_VENDOR)) {
 if (p_data->hdr.layer_specific < BTA_AV_NUM_RCB) {
      p_rcb = &p_cb->rcb[p_data->hdr.layer_specific];
      AVRC_VendorRsp(p_rcb->handle, p_data->api_vendor.label,
 &p_data->api_vendor.msg);
 }
 }
}
