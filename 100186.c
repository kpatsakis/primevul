static void cmd_list(char *tag, struct listargs *listargs)
{
    clock_t start = clock();
    char mytime[100];

    if (listargs->sel & LIST_SEL_REMOTE) {
        if (!config_getswitch(IMAPOPT_PROXYD_DISABLE_MAILBOX_REFERRALS)) {
            supports_referrals = !disable_referrals;
        }
    }

    list_callback_calls = 0;

    if (listargs->pat.count && !*(listargs->pat.data[0]) && (listargs->cmd == LIST_CMD_LIST)) {
        /* special case: query top-level hierarchy separator */
        prot_printf(imapd_out, "* LIST (\\Noselect) \"%c\" \"\"\r\n",
                    imapd_namespace.hier_sep);
    } else if (listargs->pat.count && !*(listargs->pat.data[0]) && (listargs->cmd == LIST_CMD_XLIST)) {
        /* special case: query top-level hierarchy separator */
        prot_printf(imapd_out, "* XLIST (\\Noselect) \"%c\" \"\"\r\n",
                    imapd_namespace.hier_sep);
    } else if ((listargs->cmd == LIST_CMD_LSUB) &&
               (backend_inbox || (backend_inbox = proxy_findinboxserver(imapd_userid)))) {
        /* remote inbox */
        if (list_data_remote(backend_inbox, tag, listargs, NULL))
            return;
    } else {
        list_data(listargs);
    }

    strarray_fini(&listargs->pat);
    strarray_fini(&listargs->metaitems);

    imapd_check((listargs->sel & LIST_SEL_SUBSCRIBED) ?  NULL : backend_inbox, 0);

    snprintf(mytime, sizeof(mytime), "%2.3f",
             (clock() - start) / (double) CLOCKS_PER_SEC);
    if ((listargs->sel & LIST_SEL_METADATA) && listargs->metaopts.maxsize &&
        listargs->metaopts.biggest > listargs->metaopts.maxsize) {
        prot_printf(imapd_out, "%s OK [METADATA LONGENTRIES %u] %s\r\n", tag,
                    (unsigned)listargs->metaopts.biggest, error_message(IMAP_OK_COMPLETED));
    }
    else {
        prot_printf(imapd_out, "%s OK %s (%s secs", tag,
                    error_message(IMAP_OK_COMPLETED), mytime);
        if (list_callback_calls)
            prot_printf(imapd_out, " %u calls", list_callback_calls);
        prot_printf(imapd_out, ")\r\n");
    }

    if (global_conversations) {
        conversations_abort(&global_conversations);
        global_conversations = NULL;
    }
}
