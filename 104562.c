static void mcf_fec_update(mcf_fec_state *s)
{
    uint32_t active;
    uint32_t changed;
    uint32_t mask;
    int i;

    active = s->eir & s->eimr;
    changed = active ^s->irq_state;
    for (i = 0; i < FEC_NUM_IRQ; i++) {
        mask = mcf_fec_irq_map[i];
        if (changed & mask) {
            DPRINTF("IRQ %d = %d\n", i, (active & mask) != 0);
            qemu_set_irq(s->irq[i], (active & mask) != 0);
        }
    }
    s->irq_state = active;
}
