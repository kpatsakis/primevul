static void usb_ehci_pci_init(Object *obj)
{
    DeviceClass *dc = OBJECT_GET_CLASS(DeviceClass, obj, TYPE_DEVICE);
    EHCIPCIState *i = PCI_EHCI(obj);
    EHCIState *s = &i->ehci;

    s->caps[0x09] = 0x68;        /* EECP */

    s->capsbase = 0x00;
    s->opregbase = 0x20;
    s->portscbase = 0x44;
    s->portnr = NB_PORTS;

    if (!dc->hotpluggable) {
        s->companion_enable = true;
    }

     usb_ehci_init(s, DEVICE(obj));
 }
