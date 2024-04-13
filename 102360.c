void smp_send_csrk_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tBTM_LE_LCSRK_KEYS key;
  SMP_TRACE_DEBUG("%s", __func__);
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_CSRK, false);

 if (smp_send_cmd(SMP_OPCODE_SIGN_INFO, p_cb)) {
    key.div = p_cb->div;
    key.sec_level = p_cb->sec_level;
    key.counter = 0; /* initialize the local counter */
    memcpy(key.csrk, p_cb->csrk, BT_OCTET16_LEN);
    btm_sec_save_le_key(p_cb->pairing_bda, BTM_LE_KEY_LCSRK,
 (tBTM_LE_KEY_VALUE*)&key, true);
 }

  smp_key_distribution_by_transport(p_cb, NULL);
}
