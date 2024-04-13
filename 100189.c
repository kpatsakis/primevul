static void list_data(struct listargs *listargs)
{
    canonical_list_patterns(listargs->ref, &listargs->pat);

    /* Check to see if we should only list the personal namespace */
    if (!(listargs->cmd == LIST_CMD_EXTENDED)
            && !strcmp(listargs->pat.data[0], "*")
            && config_getswitch(IMAPOPT_FOOLSTUPIDCLIENTS)) {
        strarray_set(&listargs->pat, 0, "INBOX*");
    }

    if ((listargs->ret & LIST_RET_SUBSCRIBED) &&
        (backend_inbox || (backend_inbox = proxy_findinboxserver(imapd_userid)))) {
        list_data_remotesubscriptions(listargs);
    }
    else if (listargs->sel & LIST_SEL_RECURSIVEMATCH) {
        list_data_recursivematch(listargs);
    }
    else {
        struct list_rock rock;
        memset(&rock, 0, sizeof(struct list_rock));
        rock.listargs = listargs;

        if (listargs->sel & LIST_SEL_SUBSCRIBED) {
            mboxlist_findsubmulti(&imapd_namespace, &listargs->pat,
                                  imapd_userisadmin, imapd_userid,
                                  imapd_authstate, subscribed_cb, &rock, 1);
        }
        else {
            if (config_mupdate_server) {
                /* In case we proxy to backends due to select/return criteria */
                construct_hash_table(&rock.server_table, 10, 1);
            }

            /* XXX: is there a cheaper way to figure out \Subscribed? */
            if (listargs->ret & LIST_RET_SUBSCRIBED) {
                rock.subs = mboxlist_sublist(imapd_userid);
            }

            mboxlist_findallmulti(&imapd_namespace, &listargs->pat,
                                  imapd_userisadmin, imapd_userid,
                                  imapd_authstate, list_cb, &rock);

            if (rock.subs) strarray_free(rock.subs);
            if (rock.server_table.size)
                free_hash_table(&rock.server_table, NULL);
        }

        if (rock.last_name) free(rock.last_name);
    }
}
