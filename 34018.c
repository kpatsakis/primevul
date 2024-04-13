find_same_op(const gs_composite_t *composite_action, int my_op, gs_composite_t **ppcte)
{
    const gs_pdf14trans_t *pct0 = (gs_pdf14trans_t *)composite_action;
    gs_composite_t *pct = *ppcte;

    for (;;) {
        if (pct->type->comp_id == GX_COMPOSITOR_PDF14_TRANS) {
            gs_pdf14trans_t *pct_pdf14 = (gs_pdf14trans_t *)pct;

            *ppcte = pct;
            if (pct_pdf14->params.pdf14_op != my_op)
                return COMP_ENQUEUE;
            if (pct_pdf14->params.csel == pct0->params.csel) {
                /* If the new parameters completely replace the old ones
                   then remove the old one from the queu */
                if ((pct_pdf14->params.changed & pct0->params.changed) ==
                    pct_pdf14->params.changed) {
                    return COMP_REPLACE_CURR;
                } else {
                    return COMP_ENQUEUE;
                }
            }
        } else
            return COMP_ENQUEUE;
        pct = pct->prev;
        if (pct == NULL)
            return COMP_ENQUEUE; /* Not in queue. */
    }
}
