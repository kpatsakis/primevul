eth_read(void *opaque, hwaddr addr, unsigned int size)
{
    struct xlx_ethlite *s = opaque;
    uint32_t r = 0;

    addr >>= 2;

    switch (addr)
    {
        case R_TX_GIE0:
        case R_TX_LEN0:
        case R_TX_LEN1:
        case R_TX_CTRL1:
        case R_TX_CTRL0:
        case R_RX_CTRL1:
        case R_RX_CTRL0:
            r = s->regs[addr];
            D(qemu_log("%s " TARGET_FMT_plx "=%x\n", __func__, addr * 4, r));
            break;

        default:
            r = tswap32(s->regs[addr]);
            break;
    }
    return r;
}
