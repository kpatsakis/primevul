static void vmxnet3_put_txq_descr(QEMUFile *f, void *pv, size_t size)
{
    Vmxnet3TxqDescr *r = pv;

    vmxnet3_put_ring_to_file(f, &r->tx_ring);
    vmxnet3_put_ring_to_file(f, &r->comp_ring);
    qemu_put_byte(f, r->intr_idx);
    qemu_put_be64(f, r->tx_stats_pa);
    vmxnet3_put_tx_stats_to_file(f, &r->txq_stats);
}
