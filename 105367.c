static inline bool stellaris_txpacket_complete(stellaris_enet_state *s)
{
    int framelen = stellaris_txpacket_datalen(s);
    framelen += 16;
    if (!(s->tctl & SE_TCTL_CRC)) {
        framelen += 4;
    }
    /* Cover the corner case of a 2032 byte payload with auto-CRC disabled:
     * this requires more bytes than will fit in the FIFO. It's not totally
     * clear how the h/w handles this, but if using threshold-based TX
     * it will definitely try to transmit something.
     */
    framelen = MIN(framelen, ARRAY_SIZE(s->tx_fifo));
    return s->tx_fifo_len >= framelen;
}
