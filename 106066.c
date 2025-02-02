vmxnet3_init_msi(VMXNET3State *s)
{
    PCIDevice *d = PCI_DEVICE(s);
    int res;

    res = msi_init(d, VMXNET3_MSI_OFFSET, VMXNET3_MAX_NMSIX_INTRS,
                   VMXNET3_USE_64BIT, VMXNET3_PER_VECTOR_MASK);
    if (0 > res) {
        VMW_WRPRN("Failed to initialize MSI, error %d", res);
        s->msi_used = false;
    } else {
        s->msi_used = true;
    }

    return s->msi_used;
}
