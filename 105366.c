static inline bool stellaris_tx_thr_reached(stellaris_enet_state *s)
{
    return (s->thr < 0x3f &&
            (s->tx_fifo_len >= 4 * (s->thr * 8 + 1)));
}
