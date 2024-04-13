void acpi_pcihp_device_unplug_cb(ACPIREGS *ar, qemu_irq irq, AcpiPciHpState *s,
                                 DeviceState *dev, Error **errp)
{
    PCIDevice *pdev = PCI_DEVICE(dev);
    int slot = PCI_SLOT(pdev->devfn);
    int bsel = acpi_pcihp_get_bsel(pdev->bus);
    if (bsel < 0) {
        error_setg(errp, "Unsupported bus. Bus doesn't have property '"
                   ACPI_PCIHP_PROP_BSEL "' set");
        return;
    }

    s->acpi_pcihp_pci_status[bsel].down |= (1U << slot);

    ar->gpe.sts[0] |= ACPI_PCI_HOTPLUG_STATUS;
    acpi_update_sci(ar, irq);
}
