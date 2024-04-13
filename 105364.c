static void stellaris_enet_send(stellaris_enet_state *s)
{
    int framelen = stellaris_txpacket_datalen(s);

    /* Ethernet header is in the FIFO but not in the datacount.
     * We don't implement explicit CRC, so just ignore any
     * CRC value in the FIFO.
     */
    framelen += 14;
    if ((s->tctl & SE_TCTL_PADEN) && framelen < 60) {
        memset(&s->tx_fifo[framelen + 2], 0, 60 - framelen);
        framelen = 60;
    }
    /* This MIN will have no effect unless the FIFO data is corrupt
     * (eg bad data from an incoming migration); otherwise the check
     * on the datalen at the start of writing the data into the FIFO
     * will have caught this. Silently write a corrupt half-packet,
     * which is what the hardware does in FIFO underrun situations.
     */
    framelen = MIN(framelen, ARRAY_SIZE(s->tx_fifo) - 2);
    qemu_send_packet(qemu_get_queue(s->nic), s->tx_fifo + 2, framelen);
    s->tx_fifo_len = 0;
    s->ris |= SE_INT_TXEMP;
    stellaris_enet_update(s);
    DPRINTF("Done TX\n");
}
