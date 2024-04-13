static ssize_t stellaris_enet_receive(NetClientState *nc, const uint8_t *buf, size_t size)
{
    stellaris_enet_state *s = qemu_get_nic_opaque(nc);
    int n;
    uint8_t *p;
    uint32_t crc;

    if ((s->rctl & SE_RCTL_RXEN) == 0)
        return -1;
    if (s->np >= 31) {
        return 0;
    }

    DPRINTF("Received packet len=%zu\n", size);
     n = s->next_packet + s->np;
     if (n >= 31)
         n -= 31;
 
    if (size >= sizeof(s->rx[n].data) - 6) {
        /* If the packet won't fit into the
         * emulated 2K RAM, this is reported
         * as a FIFO overrun error.
         */
        s->ris |= SE_INT_FOV;
        stellaris_enet_update(s);
        return -1;
    }

    s->np++;
     s->rx[n].len = size + 6;
     p = s->rx[n].data;
     *(p++) = (size + 6);
        memset(p, 0, (6 - size) & 3);
    }
