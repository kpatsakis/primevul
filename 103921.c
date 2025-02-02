static void ncq_err(NCQTransferState *ncq_tfs)
{
    IDEState *ide_state = &ncq_tfs->drive->port.ifs[0];

    ide_state->error = ABRT_ERR;
    ide_state->status = READY_STAT | ERR_STAT;
    ncq_tfs->drive->port_regs.scr_err |= (1 << ncq_tfs->tag);
    qemu_sglist_destroy(&ncq_tfs->sglist);
    ncq_tfs->used = 0;
}
