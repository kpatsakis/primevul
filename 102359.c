void smp_move_to_secure_connections_phase2(tSMP_CB* p_cb,
                                           tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);
  smp_sm_event(p_cb, SMP_SC_PHASE1_CMPLT_EVT, NULL);
}
