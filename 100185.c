static void cmd_getmetadata(const char *tag)
{
    int c, r = 0;
    strarray_t lists[3] = { STRARRAY_INITIALIZER,
                            STRARRAY_INITIALIZER,
                            STRARRAY_INITIALIZER };
    int is_list[3] = { 1, 1, 1 };
    int nlists = 0;
    strarray_t *options = NULL;
    strarray_t *mboxes = NULL;
    strarray_t *entries = NULL;
    strarray_t newe = STRARRAY_INITIALIZER;
    strarray_t newa = STRARRAY_INITIALIZER;
    struct buf arg1 = BUF_INITIALIZER;
    int mbox_is_pattern = 0;
    struct getmetadata_options opts = OPTS_INITIALIZER;
    annotate_state_t *astate = NULL;

    while (nlists < 3)
    {
        c = parse_metadata_string_or_list(tag, &lists[nlists], &is_list[nlists]);
        nlists++;
        if (c == '\r' || c == EOF)
            break;
    }

    /* check for CRLF */
    if (c == '\r') {

        c = prot_getc(imapd_in);

        if (c != '\n') {
            prot_printf(imapd_out,
                        "%s BAD Unexpected extra arguments to Getannotation\r\n",
                        tag);
            eatline(imapd_in, c);
            goto freeargs;
        }
    } else {
        eatline(imapd_in, c);
    }

    /*
     * We have three strings or lists of strings.  Now to figure out
     * what's what.  We have two complicating factors.  First, due to
     * a erratum in RFC5464 and our earlier misreading of the document,
     * we historically supported specifying the options *after* the
     * mailbox name.  Second, we have for a few months now supported
     * a non-standard extension where a list of mailbox names could
     * be supplied instead of just a single one.  So we have to apply
     * some rules.  We support the following syntaxes:
     *
     * --- no options
     * mailbox entry
     * mailbox (entries)
     * (mailboxes) entry
     * (mailboxes) (entries)
     *
     * --- options in the correct place (per the ABNF in RFC5464)
     * (options) mailbox entry
     * (options) mailbox (entries)
     * (options) (mailboxes) entry
     * (options) (mailboxes) (entries)
     *
     * --- options in the wrong place (per the examples in RFC5464)
     * mailbox (options) entry
     * mailbox (options) (entries)
     * (mailboxes) (options) entry
     * (mailboxes) (options) (entries)
     */
    if (nlists < 2)
        goto missingargs;
    entries = &lists[nlists-1];     /* entries always last */
    if (nlists == 2) {
        /* no options */
        mboxes = &lists[0];
        mbox_is_pattern = is_list[0];
    }
    if (nlists == 3) {
        /* options, either before or after */
        int r0 = (parse_getmetadata_options(&lists[0], NULL) > 0);
        int r1 = (parse_getmetadata_options(&lists[1], NULL) > 0);
        switch ((r1<<1)|r0) {
        case 0:
            /* neither are valid options */
            goto missingargs;
        case 1:
            /* (options) (mailboxes) */
            options = &lists[0];
            mboxes = &lists[1];
            mbox_is_pattern = is_list[1];
            break;
        case 2:
            /* (mailboxes) (options) */
            mboxes = &lists[0];
            mbox_is_pattern = is_list[0];
            options = &lists[1];
            break;
        case 3:
            /* both appear like valid options */
            prot_printf(imapd_out,
                        "%s BAD Too many option lists for Getmetadata\r\n",
                        tag);
            eatline(imapd_in, c);
            goto freeargs;
        }
    }

    if (options) parse_getmetadata_options(options, &opts);

    if (_metadata_to_annotate(entries, &newa, &newe, tag, opts.depth))
        goto freeargs;

    astate = annotate_state_new();
    annotate_state_set_auth(astate,
                            imapd_userisadmin || imapd_userisproxyadmin,
                            imapd_userid, imapd_authstate);
    if (!mboxes->count || !strcmpsafe(mboxes->data[0], NULL)) {
        r = annotate_state_set_server(astate);
        if (!r)
            r = annotate_state_fetch(astate, &newe, &newa,
                                     getmetadata_response, &opts);
    }
    else {
        struct annot_fetch_rock arock;
        arock.entries = &newe;
        arock.attribs = &newa;
        arock.callback = getmetadata_response;
        arock.cbrock = &opts;
        if (mbox_is_pattern)
            r = apply_mailbox_pattern(astate, mboxes->data[0], annot_fetch_cb, &arock);
        else
            r = apply_mailbox_array(astate, mboxes, annot_fetch_cb, &arock);
    }
    /* we didn't write anything */
    annotate_state_abort(&astate);

    getmetadata_response(NULL, 0, NULL, NULL, &opts);

    imapd_check(NULL, 0);

    if (r) {
        prot_printf(imapd_out, "%s NO %s\r\n", tag, error_message(r));
    } else if (opts.maxsize && opts.biggest > opts.maxsize) {
        prot_printf(imapd_out, "%s OK [METADATA LONGENTRIES %u] %s\r\n",
                    tag, (unsigned)opts.biggest, error_message(IMAP_OK_COMPLETED));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n",
                    tag, error_message(IMAP_OK_COMPLETED));
    }

freeargs:
    strarray_fini(&lists[0]);
    strarray_fini(&lists[1]);
    strarray_fini(&lists[2]);
    strarray_fini(&newe);
    strarray_fini(&newa);
    buf_free(&arg1);
    return;

missingargs:
    prot_printf(imapd_out, "%s BAD Missing arguments to Getmetadata\r\n", tag);
    eatline(imapd_in, c);
    goto freeargs;
}
