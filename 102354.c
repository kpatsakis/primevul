static bool pts_test_send_authentication_complete_failure(tSMP_CB* p_cb) {
 uint8_t reason = p_cb->cert_failure;
 if (reason == SMP_PAIR_AUTH_FAIL || reason == SMP_PAIR_FAIL_UNKNOWN ||
      reason == SMP_PAIR_NOT_SUPPORT || reason == SMP_PASSKEY_ENTRY_FAIL ||
      reason == SMP_REPEATED_ATTEMPTS) {
    tSMP_INT_DATA smp_int_data;
    smp_int_data.status = reason;
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &smp_int_data);
 return true;
 }
 return false;
}
