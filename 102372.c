void smp_send_app_cback(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  tSMP_EVT_DATA cb_data;
  tSMP_STATUS callback_rc;
  SMP_TRACE_DEBUG("%s p_cb->cb_evt=%d", __func__, p_cb->cb_evt);
 if (p_cb->p_callback && p_cb->cb_evt != 0) {
 switch (p_cb->cb_evt) {
 case SMP_IO_CAP_REQ_EVT:
        cb_data.io_req.auth_req = p_cb->peer_auth_req;
        cb_data.io_req.oob_data = SMP_OOB_NONE;
        cb_data.io_req.io_cap = SMP_DEFAULT_IO_CAPS;
        cb_data.io_req.max_key_size = SMP_MAX_ENC_KEY_SIZE;
        cb_data.io_req.init_keys = p_cb->local_i_key;
        cb_data.io_req.resp_keys = p_cb->local_r_key;
        SMP_TRACE_WARNING("io_cap = %d", cb_data.io_req.io_cap);
 break;

 case SMP_NC_REQ_EVT:
        cb_data.passkey = p_data->passkey;
 break;
 case SMP_SC_OOB_REQ_EVT:
        cb_data.req_oob_type = p_data->req_oob_type;
 break;
 case SMP_SC_LOC_OOB_DATA_UP_EVT:
        cb_data.loc_oob_data = p_cb->sc_oob_data.loc_oob_data;
 break;

 case SMP_BR_KEYS_REQ_EVT:
        cb_data.io_req.auth_req = 0;
        cb_data.io_req.oob_data = SMP_OOB_NONE;
        cb_data.io_req.io_cap = 0;
        cb_data.io_req.max_key_size = SMP_MAX_ENC_KEY_SIZE;
        cb_data.io_req.init_keys = SMP_BR_SEC_DEFAULT_KEY;
        cb_data.io_req.resp_keys = SMP_BR_SEC_DEFAULT_KEY;
 break;

 default:
 break;
 }

    callback_rc =
 (*p_cb->p_callback)(p_cb->cb_evt, p_cb->pairing_bda, &cb_data);

    SMP_TRACE_DEBUG("%s: callback_rc=%d  p_cb->cb_evt=%d", __func__,
                    callback_rc, p_cb->cb_evt);

 if (callback_rc == SMP_SUCCESS) {
 switch (p_cb->cb_evt) {
 case SMP_IO_CAP_REQ_EVT:
          p_cb->loc_auth_req = cb_data.io_req.auth_req;
          p_cb->local_io_capability = cb_data.io_req.io_cap;
          p_cb->loc_oob_flag = cb_data.io_req.oob_data;
          p_cb->loc_enc_size = cb_data.io_req.max_key_size;
          p_cb->local_i_key = cb_data.io_req.init_keys;
          p_cb->local_r_key = cb_data.io_req.resp_keys;

 if (!(p_cb->loc_auth_req & SMP_AUTH_BOND)) {
            SMP_TRACE_WARNING("Non bonding: No keys will be exchanged");
            p_cb->local_i_key = 0;
            p_cb->local_r_key = 0;
 }

          SMP_TRACE_WARNING(
 "rcvd auth_req: 0x%02x, io_cap: %d "
 "loc_oob_flag: %d loc_enc_size: %d, "
 "local_i_key: 0x%02x, local_r_key: 0x%02x",
              p_cb->loc_auth_req, p_cb->local_io_capability, p_cb->loc_oob_flag,
              p_cb->loc_enc_size, p_cb->local_i_key, p_cb->local_r_key);

          p_cb->secure_connections_only_mode_required =
 (btm_cb.security_mode == BTM_SEC_MODE_SC) ? true : false;

 if (p_cb->secure_connections_only_mode_required) {
            p_cb->loc_auth_req |= SMP_SC_SUPPORT_BIT;
 }

 if (!(p_cb->loc_auth_req & SMP_SC_SUPPORT_BIT) ||
              lmp_version_below(p_cb->pairing_bda, HCI_PROTO_VERSION_4_2) ||
              interop_match_addr(INTEROP_DISABLE_LE_SECURE_CONNECTIONS,
 (const RawAddress*)&p_cb->pairing_bda)) {
            p_cb->loc_auth_req &= ~SMP_KP_SUPPORT_BIT;
            p_cb->local_i_key &= ~SMP_SEC_KEY_TYPE_LK;
            p_cb->local_r_key &= ~SMP_SEC_KEY_TYPE_LK;
 }

 if (lmp_version_below(p_cb->pairing_bda, HCI_PROTO_VERSION_5_0)) {
            p_cb->loc_auth_req &= ~SMP_H7_SUPPORT_BIT;
 }

          SMP_TRACE_WARNING(
 "set auth_req: 0x%02x, local_i_key: 0x%02x, local_r_key: 0x%02x",
              p_cb->loc_auth_req, p_cb->local_i_key, p_cb->local_r_key);

          smp_sm_event(p_cb, SMP_IO_RSP_EVT, NULL);
 break;

 case SMP_BR_KEYS_REQ_EVT:
          p_cb->loc_enc_size = cb_data.io_req.max_key_size;
          p_cb->local_i_key = cb_data.io_req.init_keys;
          p_cb->local_r_key = cb_data.io_req.resp_keys;
          p_cb->loc_auth_req |= SMP_H7_SUPPORT_BIT;

          p_cb->local_i_key &= ~SMP_SEC_KEY_TYPE_LK;
          p_cb->local_r_key &= ~SMP_SEC_KEY_TYPE_LK;

          SMP_TRACE_WARNING(
 "for SMP over BR max_key_size: 0x%02x, local_i_key: 0x%02x, "
 "local_r_key: 0x%02x, p_cb->loc_auth_req: 0x%02x",
              p_cb->loc_enc_size, p_cb->local_i_key, p_cb->local_r_key,
              p_cb->loc_auth_req);

          smp_br_state_machine_event(p_cb, SMP_BR_KEYS_RSP_EVT, NULL);
 break;
 }
 }
 }

 if (!p_cb->cb_evt && p_cb->discard_sec_req) {
    p_cb->discard_sec_req = false;
    smp_sm_event(p_cb, SMP_DISCARD_SEC_REQ_EVT, NULL);
 }

  SMP_TRACE_DEBUG("%s: return", __func__);
}
