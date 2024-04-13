void smp_enc_cmpl(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t enc_enable = *(uint8_t*)p_data;
 uint8_t reason = enc_enable ? SMP_SUCCESS : SMP_ENC_FAIL;

  SMP_TRACE_DEBUG("%s", __func__);
  smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
}
