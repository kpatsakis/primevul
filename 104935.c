static void vmxnet_tx_pkt_calculate_hdr_len(struct VmxnetTxPkt *pkt)
{
    pkt->hdr_len = pkt->vec[VMXNET_TX_PKT_L2HDR_FRAG].iov_len +
        pkt->vec[VMXNET_TX_PKT_L3HDR_FRAG].iov_len;
}
