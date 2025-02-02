pvscsi_convert_sglist(PVSCSIRequest *r)
{
    uint32_t chunk_size, elmcnt = 0;
    uint64_t data_length = r->req.dataLen;
    PVSCSISGState sg = r->sg;
    while (data_length && elmcnt < PVSCSI_MAX_SG_ELEM) {
        while (!sg.resid && elmcnt++ < PVSCSI_MAX_SG_ELEM) {
            pvscsi_get_next_sg_elem(&sg);
            trace_pvscsi_convert_sglist(r->req.context, r->sg.dataAddr,
                                        r->sg.resid);
        }
        chunk_size = MIN(data_length, sg.resid);
        if (chunk_size) {
            qemu_sglist_add(&r->sgl, sg.dataAddr, chunk_size);
        }

        sg.dataAddr += chunk_size;
        data_length -= chunk_size;
        sg.resid -= chunk_size;
    }
}
