static void megasas_unmap_sgl(MegasasCmd *cmd)
{
    qemu_sglist_destroy(&cmd->qsg);
    cmd->iov_offset = 0;
}
