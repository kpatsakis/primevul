void smp_br_check_authorization_request(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s rcvs i_keys=0x%x r_keys=0x%x (i-initiator r-responder)",
                  __func__, p_cb->local_i_key, p_cb->local_r_key);

 /* In LE SC mode LK field is ignored when BR/EDR transport is used */
  p_cb->local_i_key &= ~SMP_SEC_KEY_TYPE_LK;
  p_cb->local_r_key &= ~SMP_SEC_KEY_TYPE_LK;

 /* In LE SC mode only IRK, IAI, CSRK are exchanged with the peer.
  ** Set local_r_key on master to expect only these keys. */
 if (p_cb->role == HCI_ROLE_MASTER) {
    p_cb->local_r_key &= (SMP_SEC_KEY_TYPE_ID | SMP_SEC_KEY_TYPE_CSRK);
 }

 /* Check if H7 function needs to be used for key derivation*/
 if ((p_cb->loc_auth_req & SMP_H7_SUPPORT_BIT) &&
 (p_cb->peer_auth_req & SMP_H7_SUPPORT_BIT)) {
    p_cb->key_derivation_h7_used = TRUE;
 }
  SMP_TRACE_DEBUG("%s: use h7 = %d", __func__, p_cb->key_derivation_h7_used);

  SMP_TRACE_DEBUG(
 "%s rcvs upgrades: i_keys=0x%x r_keys=0x%x (i-initiator r-responder)",
      __func__, p_cb->local_i_key, p_cb->local_r_key);

 if (/*((p_cb->peer_auth_req & SMP_AUTH_BOND) ||
          (p_cb->loc_auth_req & SMP_AUTH_BOND)) &&*/
 (p_cb->local_i_key || p_cb->local_r_key)) {
    smp_br_state_machine_event(p_cb, SMP_BR_BOND_REQ_EVT, NULL);

 /* if no peer key is expected, start master key distribution */
 if (p_cb->role == HCI_ROLE_MASTER && p_cb->local_r_key == 0)
      smp_key_distribution_by_transport(p_cb, NULL);
 } else {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = SMP_SUCCESS;
    smp_br_state_machine_event(p_cb, SMP_BR_AUTH_CMPL_EVT, &smp_int_data);
 }
}
