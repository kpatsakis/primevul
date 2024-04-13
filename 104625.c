pvscsi_build_sglist(PVSCSIState *s, PVSCSIRequest *r)
{
    PCIDevice *d = PCI_DEVICE(s);

    pci_dma_sglist_init(&r->sgl, d, 1);
    if (r->req.flags & PVSCSI_FLAG_CMD_WITH_SG_LIST) {
        pvscsi_convert_sglist(r);
    } else {
        qemu_sglist_add(&r->sgl, r->req.dataAddr, r->req.dataLen);
    }
}
