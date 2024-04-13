zsettransfer(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;
    int     code;

    check_proc(*op);
    check_ostack(zcolor_remap_one_ostack - 1);
    check_estack(1 + zcolor_remap_one_estack);
    istate->transfer_procs.red =
        istate->transfer_procs.green =
        istate->transfer_procs.blue =
        istate->transfer_procs.gray = *op;
    if ((code = gs_settransfer_remap(igs, gs_mapped_transfer, false)) < 0)
        return code;
    push_op_estack(zcolor_reset_transfer);
    pop(1);
    return zcolor_remap_one( i_ctx_p,
                             &istate->transfer_procs.gray,
                             igs->set_transfer.gray,
                             igs,
                             zcolor_remap_one_finish );
}
