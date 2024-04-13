static inline int stellaris_txpacket_datalen(stellaris_enet_state *s)
{
    return s->tx_fifo[0] | (s->tx_fifo[1] << 8);
}
