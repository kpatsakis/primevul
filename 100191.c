static void list_data_remotesubscriptions(struct listargs *listargs)
{
    /* Need to fetch subscription list from backend_inbox */
    struct list_rock rock;
    char mytag[128];

    memset(&rock, 0, sizeof(struct list_rock));
    rock.listargs = listargs;
    rock.subs = strarray_new();
    construct_hash_table(&rock.server_table, 10, 1);

    proxy_gentag(mytag, sizeof(mytag));

    if ((listargs->sel & LIST_SEL_SUBSCRIBED) &&
        !(listargs->sel & (LIST_SEL_SPECIALUSE | LIST_SEL_METADATA))) {
        /* Subscriptions are the only selection criteria.

           Send client request as-is to backend_inbox.
           Responses will be piped to the client as we build subs list.
        */
        list_data_remote(backend_inbox, mytag, listargs, rock.subs);

        /* Don't proxy to backend_inbox again */
        hash_insert(backend_inbox->hostname,
                    (void *)0xDEADBEEF, &rock.server_table);
    }
    else {
        /* Multiple selection criteria or need to return subscription info.

           Just fetch subscriptions without piping responses to the client.
           If we send entire client request, subscribed mailboxes on
           non-Inbox servers might be filtered out due to lack of metadata
           to meet the selection criteria.

           Note that we end up sending two requests to backend_inbox,
           but there doesn't appear to be any way around this.
        */
        struct listargs myargs;

        memcpy(&myargs, listargs, sizeof(struct listargs));
        myargs.sel = LIST_SEL_SUBSCRIBED;
        myargs.ret = 0;

        list_data_remote(backend_inbox, mytag, &myargs, rock.subs);
    }

    /* find */
    mboxlist_findallmulti(&imapd_namespace, &listargs->pat,
                          imapd_userisadmin, imapd_userid,
                          imapd_authstate, list_cb, &rock);

    strarray_free(rock.subs);
    free_hash_table(&rock.server_table, NULL);
    if (rock.last_name) free(rock.last_name);
}
