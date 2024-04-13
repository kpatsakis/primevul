void smp_proc_enc_info(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 uint8_t* p = (uint8_t*)p_data;

  SMP_TRACE_DEBUG("%s", __func__);
  STREAM_TO_ARRAY(p_cb->ltk, p, BT_OCTET16_LEN);

  smp_key_distribution(p_cb, NULL);
}
