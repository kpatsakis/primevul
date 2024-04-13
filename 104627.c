pvscsi_cleanup_msi(PVSCSIState *s)
{
    PCIDevice *d = PCI_DEVICE(s);

    msi_uninit(d);
}
