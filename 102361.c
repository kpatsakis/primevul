void smp_send_enc_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tBTM_LE_LENC_KEYS le_key;

  SMP_TRACE_DEBUG("%s: p_cb->loc_enc_size = %d", __func__, p_cb->loc_enc_size);
  smp_update_key_mask(p_cb, SMP_SEC_KEY_TYPE_ENC, false);

  smp_send_cmd(SMP_OPCODE_ENCRYPT_INFO, p_cb);
  smp_send_cmd(SMP_OPCODE_MASTER_ID, p_cb);

 /* save the DIV and key size information when acting as slave device */
  memcpy(le_key.ltk, p_cb->ltk, BT_OCTET16_LEN);
  le_key.div = p_cb->div;
  le_key.key_size = p_cb->loc_enc_size;
  le_key.sec_level = p_cb->sec_level;

 if ((p_cb->peer_auth_req & SMP_AUTH_BOND) &&
 (p_cb->loc_auth_req & SMP_AUTH_BOND))
    btm_sec_save_le_key(p_cb->pairing_bda, BTM_LE_KEY_LENC,
 (tBTM_LE_KEY_VALUE*)&le_key, true);

  SMP_TRACE_WARNING("%s", __func__);

  smp_key_distribution(p_cb, NULL);
}
