void esp_transfer_data(SCSIRequest *req, uint32_t len)
{
    ESPState *s = req->hba_private;

    trace_esp_transfer_data(s->dma_left, s->ti_size);
    s->async_len = len;
    s->async_buf = scsi_req_get_buf(req);
    if (s->dma_left) {
        esp_do_dma(s);
    } else if (s->dma_counter != 0 && s->ti_size <= 0) {
        /* If this was the last part of a DMA transfer then the
           completion interrupt is deferred to here.  */
        esp_dma_done(s);
    }
}
