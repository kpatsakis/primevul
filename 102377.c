void mca_ccb_do_disconn(tMCA_CCB* p_ccb, UNUSED_ATTR tMCA_CCB_EVT* p_data) {
  mca_dcb_close_by_mdl_id(p_ccb, MCA_ALL_MDL_ID);
  L2CA_DisconnectReq(p_ccb->lcid);
}
