eth_write(void *opaque, hwaddr addr,
          uint64_t val64, unsigned int size)
{
    struct xlx_ethlite *s = opaque;
    unsigned int base = 0;
    uint32_t value = val64;

    addr >>= 2;
    switch (addr) 
    {
        case R_TX_CTRL0:
        case R_TX_CTRL1:
            if (addr == R_TX_CTRL1)
                base = 0x800 / 4;

            D(qemu_log("%s addr=" TARGET_FMT_plx " val=%x\n",
                       __func__, addr * 4, value));
            if ((value & (CTRL_P | CTRL_S)) == CTRL_S) {
                qemu_send_packet(qemu_get_queue(s->nic),
                                 (void *) &s->regs[base],
                                 s->regs[base + R_TX_LEN0]);
                D(qemu_log("eth_tx %d\n", s->regs[base + R_TX_LEN0]));
                if (s->regs[base + R_TX_CTRL0] & CTRL_I)
                    eth_pulse_irq(s);
            } else if ((value & (CTRL_P | CTRL_S)) == (CTRL_P | CTRL_S)) {
                memcpy(&s->conf.macaddr.a[0], &s->regs[base], 6);
                if (s->regs[base + R_TX_CTRL0] & CTRL_I)
                    eth_pulse_irq(s);
            }

            /* We are fast and get ready pretty much immediately so
               we actually never flip the S nor P bits to one.  */
            s->regs[addr] = value & ~(CTRL_P | CTRL_S);
            break;

        /* Keep these native.  */
        case R_RX_CTRL0:
        case R_RX_CTRL1:
            if (!(value & CTRL_S)) {
                qemu_flush_queued_packets(qemu_get_queue(s->nic));
            }
            /* fall through */
        case R_TX_LEN0:
        case R_TX_LEN1:
        case R_TX_GIE0:
            D(qemu_log("%s addr=" TARGET_FMT_plx " val=%x\n",
                       __func__, addr * 4, value));
            s->regs[addr] = value;
            break;

        default:
            s->regs[addr] = tswap32(value);
            break;
    }
}
