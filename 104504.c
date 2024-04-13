static void coroutine_fn virtfs_reset(V9fsPDU *pdu)
{
    V9fsState *s = pdu->s;
    V9fsFidState *fidp = NULL;

    /* Free all fids */
    while (s->fid_list) {
        fidp = s->fid_list;
        s->fid_list = fidp->next;

        if (fidp->ref) {
            fidp->clunked = 1;
        } else {
            free_fid(pdu, fidp);
        }
    }
    if (fidp) {
        /* One or more unclunked fids found... */
        error_report("9pfs:%s: One or more uncluncked fids "
                     "found during reset", __func__);
    }
}
