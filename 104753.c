static int eth_can_rx(NetClientState *nc)
{
    struct xlx_ethlite *s = qemu_get_nic_opaque(nc);
    unsigned int rxbase = s->rxbuf * (0x800 / 4);

    return !(s->regs[rxbase + R_RX_CTRL0] & CTRL_S);
}
