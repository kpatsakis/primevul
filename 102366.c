void smp_br_process_link_key(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tSMP_STATUS status = SMP_PAIR_FAIL_UNKNOWN;

  SMP_TRACE_DEBUG("%s", __func__);
 if (!smp_calculate_long_term_key_from_link_key(p_cb)) {
    SMP_TRACE_ERROR("%s: failed", __func__);
    smp_sm_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &status);
 return;
 }

  SMP_TRACE_DEBUG("%s: LTK derivation from LK successfully completed",
                  __func__);
  smp_save_secure_connections_long_term_key(p_cb);
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_ENC, false);
  smp_br_select_next_key(p_cb, NULL);
}
