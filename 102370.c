void smp_proc_rand(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = (uint8_t*)p_data;
 uint8_t reason = SMP_INVALID_PARAMETERS;

  SMP_TRACE_DEBUG("%s", __func__);

 if (smp_command_has_invalid_parameters(p_cb)) {
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return;
 }

 /* save the SRand for comparison */
  STREAM_TO_ARRAY(p_cb->rrand, p, BT_OCTET16_LEN);
}
