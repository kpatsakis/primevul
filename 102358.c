void smp_fast_conn_param(tSMP_CB* p_cb, tSMP_INT_DATA* p_data) {
 /* Disable L2CAP connection parameter updates while bonding since
     some peripherals are not able to revert to fast connection parameters
     during the start of service discovery. Connection paramter updates
     get enabled again once service discovery completes. */
  L2CA_EnableUpdateBleConnParams(p_cb->pairing_bda, false);
}
