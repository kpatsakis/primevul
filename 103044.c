static uint8_t lsi_get_msgbyte(LSIState *s)
{
    uint8_t data;
    pci_dma_read(PCI_DEVICE(s), s->dnad, &data, 1);
    s->dnad++;
    s->dbc--;
    return data;
}
