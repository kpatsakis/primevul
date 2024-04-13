static bool pts_test_send_authentication_complete_failure(tSMP_CB* p_cb) {
 uint8_t reason = p_cb->cert_failure;
 if (reason == SMP_PAIR_AUTH_FAIL || reason == SMP_PAIR_FAIL_UNKNOWN ||
      reason == SMP_PAIR_NOT_SUPPORT || reason == SMP_PASSKEY_ENTRY_FAIL ||
      reason == SMP_REPEATED_ATTEMPTS) {
    smp_sm_event(p_cb, SMP_AUTH_CMPL_EVT, &reason);
 return true;
 }
 return false;
}
