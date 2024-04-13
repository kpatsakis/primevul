static int vmxnet3_get_txq_descr(QEMUFile *f, void *pv, size_t size)
{
    Vmxnet3TxqDescr *r = pv;

    vmxnet3_get_ring_from_file(f, &r->tx_ring);
    vmxnet3_get_ring_from_file(f, &r->comp_ring);
    r->intr_idx = qemu_get_byte(f);
    r->tx_stats_pa = qemu_get_be64(f);

    vmxnet3_get_tx_stats_from_file(f, &r->txq_stats);

    return 0;
}
