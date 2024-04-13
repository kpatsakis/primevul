vmxnet3_msix_load(QEMUFile *f, void *opaque, int version_id)
{
    PCIDevice *d = PCI_DEVICE(opaque);
    msix_load(d, f);
    return 0;
}
