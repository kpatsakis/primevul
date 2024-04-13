static void vmxnet3_get_tx_stats_from_file(QEMUFile *f,
    struct UPT1_TxStats *tx_stat)
{
    tx_stat->TSOPktsTxOK = qemu_get_be64(f);
    tx_stat->TSOBytesTxOK = qemu_get_be64(f);
    tx_stat->ucastPktsTxOK = qemu_get_be64(f);
    tx_stat->ucastBytesTxOK = qemu_get_be64(f);
    tx_stat->mcastPktsTxOK = qemu_get_be64(f);
    tx_stat->mcastBytesTxOK = qemu_get_be64(f);
    tx_stat->bcastPktsTxOK = qemu_get_be64(f);
    tx_stat->bcastBytesTxOK = qemu_get_be64(f);
    tx_stat->pktsTxError = qemu_get_be64(f);
    tx_stat->pktsTxDiscard = qemu_get_be64(f);
}
