zprocesscolors(i_ctx_t * i_ctx_p)
{
    os_ptr  op = osp;

    push(1);
    make_int(op, gs_currentdevice(igs)->color_info.num_components);
    return 0;
}
