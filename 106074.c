static void vmxnet3_pci_uninit(PCIDevice *pci_dev)
{
    DeviceState *dev = DEVICE(pci_dev);
    VMXNET3State *s = VMXNET3(pci_dev);

    VMW_CBPRN("Starting uninit...");

    unregister_savevm(dev, "vmxnet3-msix", s);

    vmxnet3_net_uninit(s);

    vmxnet3_cleanup_msix(s);

    vmxnet3_cleanup_msi(s);

    memory_region_destroy(&s->bar0);
    memory_region_destroy(&s->bar1);
    memory_region_destroy(&s->msix_bar);
}
