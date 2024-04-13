static void ahci_set_signature(AHCIDevice *ad, uint32_t sig)
{
    IDEState *s = &ad->port.ifs[0];
    s->hcyl = sig >> 24 & 0xFF;
    s->lcyl = sig >> 16 & 0xFF;
    s->sector = sig >> 8 & 0xFF;
    s->nsector = sig & 0xFF;

    DPRINTF(ad->port_no, "set hcyl:lcyl:sect:nsect = 0x%08x\n", sig);
}
