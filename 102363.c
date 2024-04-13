void smp_start_enc(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tBTM_STATUS cmd;

  SMP_TRACE_DEBUG("%s", __func__);
 if (p_data != NULL)
    cmd = btm_ble_start_encrypt(p_cb->pairing_bda, true, p_data->key.p_data);
 else
    cmd = btm_ble_start_encrypt(p_cb->pairing_bda, false, NULL);

 if (cmd != BTM_CMD_STARTED && cmd != BTM_BUSY) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_ENC_FAIL;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
 }
}
