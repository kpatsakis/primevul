static AHCICmdHdr *get_cmd_header(AHCIState *s, uint8_t port, uint8_t slot)
{
    if (port >= s->ports || slot >= AHCI_MAX_CMDS) {
        return NULL;
    }

    return s->dev[port].lst ? &((AHCICmdHdr *)s->dev[port].lst)[slot] : NULL;
}
