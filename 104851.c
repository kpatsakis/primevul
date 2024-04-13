static void vmxnet3_get_rx_stats_from_file(QEMUFile *f,
    struct UPT1_RxStats *rx_stat)
{
    rx_stat->LROPktsRxOK = qemu_get_be64(f);
    rx_stat->LROBytesRxOK = qemu_get_be64(f);
    rx_stat->ucastPktsRxOK = qemu_get_be64(f);
    rx_stat->ucastBytesRxOK = qemu_get_be64(f);
    rx_stat->mcastPktsRxOK = qemu_get_be64(f);
    rx_stat->mcastBytesRxOK = qemu_get_be64(f);
    rx_stat->bcastPktsRxOK = qemu_get_be64(f);
    rx_stat->bcastBytesRxOK = qemu_get_be64(f);
    rx_stat->pktsRxOutOfBuf = qemu_get_be64(f);
    rx_stat->pktsRxError = qemu_get_be64(f);
}
