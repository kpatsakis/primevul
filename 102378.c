void mca_ccb_free_msg(UNUSED_ATTR tMCA_CCB* p_ccb, tMCA_CCB_EVT* p_data) {
  osi_free(p_data);
}
