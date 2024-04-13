void smp_both_have_public_keys(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

 /* invokes DHKey computation */
  smp_compute_dhkey(p_cb);

 /* on slave side invokes sending local public key to the peer */
 if (p_cb->role == HCI_ROLE_SLAVE) smp_send_pair_public_key(p_cb, NULL);

  smp_sm_event(p_cb, SMP_SC_DHKEY_CMPLT_EVT, NULL);
}
