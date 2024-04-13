static void vmxnet3_put_tx_stats_to_file(QEMUFile *f,
    struct UPT1_TxStats *tx_stat)
{
    qemu_put_be64(f, tx_stat->TSOPktsTxOK);
    qemu_put_be64(f, tx_stat->TSOBytesTxOK);
    qemu_put_be64(f, tx_stat->ucastPktsTxOK);
    qemu_put_be64(f, tx_stat->ucastBytesTxOK);
    qemu_put_be64(f, tx_stat->mcastPktsTxOK);
    qemu_put_be64(f, tx_stat->mcastBytesTxOK);
    qemu_put_be64(f, tx_stat->bcastPktsTxOK);
    qemu_put_be64(f, tx_stat->bcastBytesTxOK);
    qemu_put_be64(f, tx_stat->pktsTxError);
    qemu_put_be64(f, tx_stat->pktsTxDiscard);
}
