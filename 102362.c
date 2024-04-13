void smp_send_keypress_notification(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  p_cb->local_keypress_notification = p_data->status;
  smp_send_cmd(SMP_OPCODE_PAIR_KEYPR_NOTIF, p_cb);
}
