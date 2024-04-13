void smp_br_pairing_complete(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
  SMP_TRACE_DEBUG("%s", __func__);

 if (p_cb->total_tx_unacked == 0) {
 /* process the pairing complete */
    smp_proc_pairing_cmpl(p_cb);
 }
}
