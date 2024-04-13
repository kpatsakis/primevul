vmxnet3_write_config(PCIDevice *pci_dev, uint32_t addr, uint32_t val, int len)
{
    pci_default_write_config(pci_dev, addr, val, len);
    msix_write_config(pci_dev, addr, val, len);
    msi_write_config(pci_dev, addr, val, len);
}
