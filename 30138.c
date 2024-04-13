zsave(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    uint space = icurrent_space;
    vm_save_t *vmsave;
    ulong sid;
    int code;
    gs_gstate *prev;

    if (I_VALIDATE_BEFORE_SAVE)
        ivalidate_clean_spaces(i_ctx_p);
    ialloc_set_space(idmemory, avm_local);
    vmsave = ialloc_struct(vm_save_t, &st_vm_save, "zsave");
    ialloc_set_space(idmemory, space);
    if (vmsave == 0)
        return_error(gs_error_VMerror);
    vmsave->gsave = NULL; /* Ensure constructed enough to destroy safely */
    code = alloc_save_state(idmemory, vmsave, &sid);
    if (code < 0)
        return code;
    if (sid == 0) {
        ifree_object(vmsave, "zsave");
        return_error(gs_error_VMerror);
    }
    if_debug2m('u', imemory, "[u]vmsave 0x%lx, id = %lu\n",
               (ulong) vmsave, (ulong) sid);
    code = gs_gsave_for_save(igs, &prev);
    if (code < 0)
        return code;
    code = gs_gsave(igs);
    if (code < 0)
        return code;
    vmsave->gsave = prev;
    push(1);
    make_tav(op, t_save, 0, saveid, sid);
    if (I_VALIDATE_AFTER_SAVE)
        ivalidate_clean_spaces(i_ctx_p);
    return 0;
}
