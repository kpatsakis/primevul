ztoken_continue(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    scanner_state *pstate;

    check_stype(*op, st_scanner_state_dynamic);
    pstate = r_ptr(op, scanner_state);
    return token_continue(i_ctx_p, pstate, false);
}
