static void stellaris_enet_update(stellaris_enet_state *s)
{
    qemu_set_irq(s->irq, (s->ris & s->im) != 0);
}
