static void esp_dma_done(ESPState *s)
{
    s->rregs[ESP_RSTAT] |= STAT_TC;
    s->rregs[ESP_RINTR] = INTR_BS;
    s->rregs[ESP_RSEQ] = 0;
    s->rregs[ESP_RFLAGS] = 0;
    s->rregs[ESP_TCLO] = 0;
    s->rregs[ESP_TCMID] = 0;
    s->rregs[ESP_TCHI] = 0;
    esp_raise_irq(s);
}
