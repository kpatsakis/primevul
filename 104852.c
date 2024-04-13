static int vmxnet3_get_rxq_descr(QEMUFile *f, void *pv, size_t size)
{
    Vmxnet3RxqDescr *r = pv;
    int i;

    for (i = 0; i < VMXNET3_RX_RINGS_PER_QUEUE; i++) {
        vmxnet3_get_ring_from_file(f, &r->rx_ring[i]);
    }

    vmxnet3_get_ring_from_file(f, &r->comp_ring);
    r->intr_idx = qemu_get_byte(f);
    r->rx_stats_pa = qemu_get_be64(f);

    vmxnet3_get_rx_stats_from_file(f, &r->rxq_stats);

    return 0;
}
