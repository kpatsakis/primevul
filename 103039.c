static void lsi_do_dma(LSIState *s, int out)
{
    uint32_t count;
    dma_addr_t addr;
    SCSIDevice *dev;

    assert(s->current);
    if (!s->current->dma_len) {
        /* Wait until data is available.  */
        trace_lsi_do_dma_unavailable();
        return;
    }

    dev = s->current->req->dev;
    assert(dev);

    count = s->dbc;
    if (count > s->current->dma_len)
        count = s->current->dma_len;

    addr = s->dnad;
    /* both 40 and Table Indirect 64-bit DMAs store upper bits in dnad64 */
    if (lsi_dma_40bit(s) || lsi_dma_ti64bit(s))
        addr |= ((uint64_t)s->dnad64 << 32);
    else if (s->dbms)
        addr |= ((uint64_t)s->dbms << 32);
    else if (s->sbms)
        addr |= ((uint64_t)s->sbms << 32);

    trace_lsi_do_dma(addr, count);
    s->csbc += count;
    s->dnad += count;
    s->dbc -= count;
     if (s->current->dma_buf == NULL) {
        s->current->dma_buf = scsi_req_get_buf(s->current->req);
    }
    /* ??? Set SFBR to first data byte.  */
    if (out) {
        lsi_mem_read(s, addr, s->current->dma_buf, count);
    } else {
        lsi_mem_write(s, addr, s->current->dma_buf, count);
    }
    s->current->dma_len -= count;
    if (s->current->dma_len == 0) {
        s->current->dma_buf = NULL;
        scsi_req_continue(s->current->req);
    } else {
        s->current->dma_buf += count;
        lsi_resume_script(s);
    }
}
