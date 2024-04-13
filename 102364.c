static void smp_update_key_mask(tSMP_CB* p_cb, uint8_t key_type, bool recv) {
  SMP_TRACE_DEBUG(
 "%s before update role=%d recv=%d local_i_key = %02x, local_r_key = %02x",
      __func__, p_cb->role, recv, p_cb->local_i_key, p_cb->local_r_key);

 if (((p_cb->le_secure_connections_mode_is_used) || (p_cb->smp_over_br)) &&
 ((key_type == SMP_SEC_KEY_TYPE_ENC) ||
 (key_type == SMP_SEC_KEY_TYPE_LK))) {
 /* in LE SC mode LTK, CSRK and BR/EDR LK are derived locally instead of
    ** being exchanged with the peer */
    p_cb->local_i_key &= ~key_type;
    p_cb->local_r_key &= ~key_type;
 } else if (p_cb->role == HCI_ROLE_SLAVE) {
 if (recv)
      p_cb->local_i_key &= ~key_type;
 else
      p_cb->local_r_key &= ~key_type;
 } else {
 if (recv)
      p_cb->local_r_key &= ~key_type;
 else
      p_cb->local_i_key &= ~key_type;
 }

  SMP_TRACE_DEBUG("updated local_i_key = %02x, local_r_key = %02x",
                  p_cb->local_i_key, p_cb->local_r_key);
}
