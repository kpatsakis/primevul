vmxnet3_msix_save(QEMUFile *f, void *opaque)
{
    PCIDevice *d = PCI_DEVICE(opaque);
    msix_save(d, f);
}
