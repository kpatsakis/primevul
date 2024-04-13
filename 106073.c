static int vmxnet3_pci_init(PCIDevice *pci_dev)
{
    DeviceState *dev = DEVICE(pci_dev);
    VMXNET3State *s = VMXNET3(pci_dev);

    VMW_CBPRN("Starting init...");

    memory_region_init_io(&s->bar0, OBJECT(s), &b0_ops, s,
                          "vmxnet3-b0", VMXNET3_PT_REG_SIZE);
    pci_register_bar(pci_dev, VMXNET3_BAR0_IDX,
                     PCI_BASE_ADDRESS_SPACE_MEMORY, &s->bar0);

    memory_region_init_io(&s->bar1, OBJECT(s), &b1_ops, s,
                          "vmxnet3-b1", VMXNET3_VD_REG_SIZE);
    pci_register_bar(pci_dev, VMXNET3_BAR1_IDX,
                     PCI_BASE_ADDRESS_SPACE_MEMORY, &s->bar1);

    memory_region_init(&s->msix_bar, OBJECT(s), "vmxnet3-msix-bar",
                       VMXNET3_MSIX_BAR_SIZE);
    pci_register_bar(pci_dev, VMXNET3_MSIX_BAR_IDX,
                     PCI_BASE_ADDRESS_SPACE_MEMORY, &s->msix_bar);

    vmxnet3_reset_interrupt_states(s);

    /* Interrupt pin A */
    pci_dev->config[PCI_INTERRUPT_PIN] = 0x01;

    if (!vmxnet3_init_msix(s)) {
        VMW_WRPRN("Failed to initialize MSI-X, configuration is inconsistent.");
    }

    if (!vmxnet3_init_msi(s)) {
        VMW_WRPRN("Failed to initialize MSI, configuration is inconsistent.");
    }

    vmxnet3_net_init(s);

    register_savevm(dev, "vmxnet3-msix", -1, 1,
                    vmxnet3_msix_save, vmxnet3_msix_load, s);

    add_boot_device_path(s->conf.bootindex, dev, "/ethernet-phy@0");

    return 0;
}
