static void cirrus_vga_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = pci_cirrus_vga_realize;
    k->romfile = VGABIOS_CIRRUS_FILENAME;
    k->vendor_id = PCI_VENDOR_ID_CIRRUS;
    k->device_id = CIRRUS_ID_CLGD5446;
    k->class_id = PCI_CLASS_DISPLAY_VGA;
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
    dc->desc = "Cirrus CLGD 54xx VGA";
    dc->vmsd = &vmstate_pci_cirrus_vga;
    dc->props = pci_vga_cirrus_properties;
    dc->hotpluggable = false;
}
