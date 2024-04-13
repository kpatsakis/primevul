static void cmd_expunge(char *tag, char *sequence)
{
    modseq_t old;
    modseq_t new;
    int r = 0;

    if (backend_current) {
        /* remote mailbox */
        if (sequence) {
            prot_printf(backend_current->out, "%s UID Expunge %s\r\n", tag,
                        sequence);
        } else {
            prot_printf(backend_current->out, "%s Expunge\r\n", tag);
        }
        pipe_including_tag(backend_current, tag, 0);
        return;
    }

    /* local mailbox */
    if (!index_hasrights(imapd_index, ACL_EXPUNGE))
        r = IMAP_PERMISSION_DENIED;

    old = index_highestmodseq(imapd_index);

    if (!r) r = index_expunge(imapd_index, sequence, 1);
    /* tell expunges */
    if (!r) index_tellchanges(imapd_index, 1, sequence ? 1 : 0, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
        return;
    }

    new = index_highestmodseq(imapd_index);

    prot_printf(imapd_out, "%s OK ", tag);
    if (new > old)
        prot_printf(imapd_out, "[HIGHESTMODSEQ " MODSEQ_FMT "] ", new);
    prot_printf(imapd_out, "%s\r\n", error_message(IMAP_OK_COMPLETED));
}
