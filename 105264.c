static void iscsi_block_init(void)
{
    bdrv_register(&bdrv_iscsi);
    qemu_add_opts(&qemu_iscsi_opts);
}
