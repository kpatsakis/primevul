void mca_ccb_snd_req(tMCA_CCB* p_ccb, tMCA_CCB_EVT* p_data) {
  tMCA_CCB_MSG* p_msg = (tMCA_CCB_MSG*)p_data;
 uint8_t *p, *p_start;
 bool is_abort = false;
  tMCA_DCB* p_dcb;

  MCA_TRACE_DEBUG("mca_ccb_snd_req cong=%d req=%d", p_ccb->cong,
                  p_msg->op_code);
 /* check for abort request */
 if ((p_ccb->status == MCA_CCB_STAT_PENDING) &&
 (p_msg->op_code == MCA_OP_MDL_ABORT_REQ)) {
    p_dcb = mca_dcb_by_hdl(p_ccb->p_tx_req->dcb_idx);
 /* the Abort API does not have the associated mdl_id.
     * Get the mdl_id in dcb to compose the request */
    p_msg->mdl_id = p_dcb->mdl_id;
    mca_dcb_event(p_dcb, MCA_DCB_API_CLOSE_EVT, NULL);
    osi_free_and_reset((void**)&p_ccb->p_tx_req);
    p_ccb->status = MCA_CCB_STAT_NORM;
    is_abort = true;
 }

 /* no pending outgoing messages or it's an abort request for a pending data
   * channel */
 if ((!p_ccb->p_tx_req) || is_abort) {
    p_ccb->p_tx_req = p_msg;
 if (!p_ccb->cong) {
      BT_HDR* p_pkt = (BT_HDR*)osi_malloc(MCA_CTRL_MTU + sizeof(BT_HDR));

      p_pkt->offset = L2CAP_MIN_OFFSET;
      p = p_start = (uint8_t*)(p_pkt + 1) + L2CAP_MIN_OFFSET;
 *p++ = p_msg->op_code;
      UINT16_TO_BE_STREAM(p, p_msg->mdl_id);
 if (p_msg->op_code == MCA_OP_MDL_CREATE_REQ) {
 *p++ = p_msg->mdep_id;
 *p++ = p_msg->param;
 }
      p_msg->hdr.layer_specific = true; /* mark this message as sent */
      p_pkt->len = p - p_start;
      L2CA_DataWrite(p_ccb->lcid, p_pkt);
 period_ms_t interval_ms = p_ccb->p_rcb->reg.rsp_tout * 1000;
      alarm_set_on_mloop(p_ccb->mca_ccb_timer, interval_ms,
                         mca_ccb_timer_timeout, p_ccb);
 }
 /* else the L2CAP channel is congested. keep the message to be sent later */
 } else {
    MCA_TRACE_WARNING("dropping api req");
    osi_free(p_data);
 }
}
