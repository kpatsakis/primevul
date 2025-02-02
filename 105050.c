static uint32_t get_cmd(ESPState *s, uint8_t *buf, uint8_t buflen)
{
    uint32_t dmalen;
    int target;

    target = s->wregs[ESP_WBUSID] & BUSID_DID;
    if (s->dma) {
        dmalen = s->rregs[ESP_TCLO];
        dmalen |= s->rregs[ESP_TCMID] << 8;
        dmalen |= s->rregs[ESP_TCHI] << 16;
        if (dmalen > buflen) {
            return 0;
        }
        s->dma_memory_read(s->dma_opaque, buf, dmalen);
    } else {
        dmalen = s->ti_size;
        if (dmalen > TI_BUFSZ) {
            return 0;
        }
        memcpy(buf, s->ti_buf, dmalen);
        buf[0] = buf[2] >> 5;
    }
    trace_esp_get_cmd(dmalen, target);

    s->ti_size = 0;
    s->ti_rptr = 0;
    s->ti_wptr = 0;

    if (s->current_req) {
        /* Started a new command before the old one finished.  Cancel it.  */
        scsi_req_cancel(s->current_req);
        s->async_len = 0;
    }

    s->current_dev = scsi_device_find(&s->bus, 0, target, 0);
    if (!s->current_dev) {
        s->rregs[ESP_RSTAT] = 0;
        s->rregs[ESP_RINTR] = INTR_DC;
        s->rregs[ESP_RSEQ] = SEQ_0;
        esp_raise_irq(s);
        return 0;
    }
    return dmalen;
}
