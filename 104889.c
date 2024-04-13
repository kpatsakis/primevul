static void vmxnet3_put_rxq_descr(QEMUFile *f, void *pv, size_t size)
{
    Vmxnet3RxqDescr *r = pv;
    int i;

    for (i = 0; i < VMXNET3_RX_RINGS_PER_QUEUE; i++) {
        vmxnet3_put_ring_to_file(f, &r->rx_ring[i]);
    }

    vmxnet3_put_ring_to_file(f, &r->comp_ring);
    qemu_put_byte(f, r->intr_idx);
    qemu_put_be64(f, r->rx_stats_pa);
    vmxnet3_put_rx_stats_to_file(f, &r->rxq_stats);
}
