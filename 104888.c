static void vmxnet3_put_rx_stats_to_file(QEMUFile *f,
    struct UPT1_RxStats *rx_stat)
{
    qemu_put_be64(f, rx_stat->LROPktsRxOK);
    qemu_put_be64(f, rx_stat->LROBytesRxOK);
    qemu_put_be64(f, rx_stat->ucastPktsRxOK);
    qemu_put_be64(f, rx_stat->ucastBytesRxOK);
    qemu_put_be64(f, rx_stat->mcastPktsRxOK);
    qemu_put_be64(f, rx_stat->mcastBytesRxOK);
    qemu_put_be64(f, rx_stat->bcastPktsRxOK);
    qemu_put_be64(f, rx_stat->bcastBytesRxOK);
    qemu_put_be64(f, rx_stat->pktsRxOutOfBuf);
    qemu_put_be64(f, rx_stat->pktsRxError);
}
