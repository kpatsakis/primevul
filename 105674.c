static void ne2000_instance_init(Object *obj)
{
    PCIDevice *pci_dev = PCI_DEVICE(obj);
    PCINE2000State *d = DO_UPCAST(PCINE2000State, dev, pci_dev);
    NE2000State *s = &d->ne2000;

    device_add_bootindex_property(obj, &s->c.bootindex,
                                  "bootindex", "/ethernet-phy@0",
                                  &pci_dev->qdev, NULL);
}
