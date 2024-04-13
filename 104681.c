pvscsi_uninit(PCIDevice *pci_dev)
{
    PVSCSIState *s = PVSCSI(pci_dev);

    trace_pvscsi_state("uninit");
    qemu_bh_delete(s->completion_worker);

    pvscsi_cleanup_msi(s);
}
