static void cmd_append(char *tag, char *name, const char *cur_name)
{
    int c;
    static struct buf arg;
    time_t now = time(NULL);
    quota_t qdiffs[QUOTA_NUMRESOURCES] = QUOTA_DIFFS_INITIALIZER;
    unsigned size;
    int sync_seen = 0;
    int r;
    int i;
    struct appendstate appendstate;
    unsigned long uidvalidity = 0;
    long doappenduid = 0;
    const char *parseerr = NULL, *url = NULL;
    struct appendstage *curstage;
    mbentry_t *mbentry = NULL;

    /* See if we can append */
    char *intname = mboxname_from_external(name, &imapd_namespace, imapd_userid);
    r = mlookup(tag, name, intname, &mbentry);

    if (!r && (mbentry->mbtype & MBTYPE_REMOTE)) {
        /* remote mailbox */
        struct backend *s = NULL;

        if (supports_referrals) {
            imapd_refer(tag, mbentry->server, name);
            /* Eat the argument */
            eatline(imapd_in, prot_getc(imapd_in));
            mboxlist_entry_free(&mbentry);
            free(intname);
            return;
        }

        s = proxy_findserver(mbentry->server, &imap_protocol,
                             proxy_userid, &backend_cached,
                             &backend_current, &backend_inbox, imapd_in);
        if (!s) r = IMAP_SERVER_UNAVAILABLE;

        mboxlist_entry_free(&mbentry);

        imapd_check(s, 0);

        if (!r) {
            int is_active = 1;
            s->context = (void*) &is_active;
            if (imapd_index) {
                const char *mboxname = index_mboxname(imapd_index);
                prot_printf(s->out, "%s Localappend {" SIZE_T_FMT "+}\r\n%s"
                            " {" SIZE_T_FMT "+}\r\n%s ",
                            tag, strlen(name), name,
                            strlen(mboxname), mboxname);
            } else {
                prot_printf(s->out, "%s Localappend {" SIZE_T_FMT "+}\r\n%s"
                            " \"\" ", tag, strlen(name), name);
            }
            if (!(r = pipe_command(s, 16384))) {
                pipe_including_tag(s, tag, 0);
            }
            s->context = NULL;
        } else {
            eatline(imapd_in, prot_getc(imapd_in));
        }

        if (r) {
            prot_printf(imapd_out, "%s NO %s\r\n", tag,
                        prot_error(imapd_in) ? prot_error(imapd_in) :
                        error_message(r));
        }
        free(intname);

        return;
    }

    mboxlist_entry_free(&mbentry);

    /* local mailbox */
    if (!r) {
        qdiffs[QUOTA_MESSAGE] = 1;
        r = append_check(intname, imapd_authstate, ACL_INSERT, ignorequota ? NULL : qdiffs);
    }
    if (r) {
        eatline(imapd_in, ' ');
        prot_printf(imapd_out, "%s NO %s%s\r\n",
                    tag,
                    (r == IMAP_MAILBOX_NONEXISTENT &&
                     mboxlist_createmailboxcheck(intname, 0, 0,
                                                 imapd_userisadmin,
                                                 imapd_userid, imapd_authstate,
                                                 NULL, NULL, 0) == 0)
                    ? "[TRYCREATE] " : "", error_message(r));
        free(intname);
        return;
    }

    c = ' '; /* just parsed a space */
    /* we loop, to support MULTIAPPEND */
    while (!r && c == ' ') {
        curstage = xzmalloc(sizeof(*curstage));
        ptrarray_push(&stages, curstage);

        /* now parsing "append-opts" in the ABNF */

        /* Parse flags */
        c = getword(imapd_in, &arg);
        if  (c == '(' && !arg.s[0]) {
            strarray_init(&curstage->flags);
            do {
                c = getword(imapd_in, &arg);
                if (!curstage->flags.count && !arg.s[0] && c == ')') break; /* empty list */
                if (!isokflag(arg.s, &sync_seen)) {
                    parseerr = "Invalid flag in Append command";
                    r = IMAP_PROTOCOL_ERROR;
                    goto done;
                }
                strarray_append(&curstage->flags, arg.s);
            } while (c == ' ');
            if (c != ')') {
                parseerr =
                    "Missing space or ) after flag name in Append command";
                r = IMAP_PROTOCOL_ERROR;
                goto done;
            }
            c = prot_getc(imapd_in);
            if (c != ' ') {
                parseerr = "Missing space after flag list in Append command";
                r = IMAP_PROTOCOL_ERROR;
                goto done;
            }
            c = getword(imapd_in, &arg);
        }

        /* Parse internaldate */
        if (c == '\"' && !arg.s[0]) {
            prot_ungetc(c, imapd_in);
            c = getdatetime(&(curstage->internaldate));
            if (c != ' ') {
                parseerr = "Invalid date-time in Append command";
                r = IMAP_PROTOCOL_ERROR;
                goto done;
            }
            c = getword(imapd_in, &arg);
        }

        /* try to parse a sequence of "append-ext" */
        for (;;) {
            if (!strcasecmp(arg.s, "ANNOTATION")) {
                /* RFC5257 */
                if (c != ' ') {
                    parseerr = "Missing annotation data in Append command";
                    r = IMAP_PROTOCOL_ERROR;
                    goto done;
                }
                c = parse_annotate_store_data(tag,
                                              /*permessage_flag*/1,
                                              &curstage->annotations);
                if (c == EOF) {
                    eatline(imapd_in, c);
                    goto cleanup;
                }
                qdiffs[QUOTA_ANNOTSTORAGE] += sizeentryatts(curstage->annotations);
                c = getword(imapd_in, &arg);
            }
            else
                break;  /* not a known extension keyword */
        }

        /* Stage the message */
        curstage->f = append_newstage(intname, now, stages.count, &(curstage->stage));
        if (!curstage->f) {
            r = IMAP_IOERROR;
            goto done;
        }

        /* now parsing "append-data" in the ABNF */

        if (!strcasecmp(arg.s, "CATENATE")) {
            if (c != ' ' || (c = prot_getc(imapd_in) != '(')) {
                parseerr = "Missing message part(s) in Append command";
                r = IMAP_PROTOCOL_ERROR;
                goto done;
            }

            /* Catenate the message part(s) to stage */
            size = 0;
            r = append_catenate(curstage->f, cur_name, &size,
                                &(curstage->binary), &parseerr, &url);
            if (r) goto done;
        }
        else {
            /* Read size from literal */
            r = getliteralsize(arg.s, c, &size, &(curstage->binary), &parseerr);
            if (!r && size == 0) r = IMAP_ZERO_LENGTH_LITERAL;
            if (r) goto done;

            /* Copy message to stage */
            r = message_copy_strict(imapd_in, curstage->f, size, curstage->binary);
        }
        qdiffs[QUOTA_STORAGE] += size;
        /* If this is a non-BINARY message, close the stage file.
         * Otherwise, leave it open so we can encode the binary parts.
         *
         * XXX  For BINARY MULTIAPPEND, we may have to close the stage files
         * anyways to avoid too many open files.
         */
        if (!curstage->binary) {
            fclose(curstage->f);
            curstage->f = NULL;
        }

        /* if we see a SP, we're trying to append more than one message */

        /* Parse newline terminating command */
        c = prot_getc(imapd_in);
    }

 done:
    if (r) {
        eatline(imapd_in, c);
    } else {
        /* we should be looking at the end of the line */
        if (c == '\r') c = prot_getc(imapd_in);
        if (c != '\n') {
            parseerr = "junk after literal";
            r = IMAP_PROTOCOL_ERROR;
            eatline(imapd_in, c);
        }
    }

    /* Append from the stage(s) */
    if (!r) {
        qdiffs[QUOTA_MESSAGE] = stages.count;
        r = append_setup(&appendstate, intname,
                         imapd_userid, imapd_authstate, ACL_INSERT,
                         ignorequota ? NULL : qdiffs, &imapd_namespace,
                         (imapd_userisadmin || imapd_userisproxyadmin),
                         EVENT_MESSAGE_APPEND);
    }
    if (!r) {
        struct body *body;

        doappenduid = (appendstate.myrights & ACL_READ);
        uidvalidity = append_uidvalidity(&appendstate);

        for (i = 0; !r && i < stages.count ; i++) {
            curstage = stages.data[i];
            body = NULL;
            if (curstage->binary) {
                r = message_parse_binary_file(curstage->f, &body);
                fclose(curstage->f);
                curstage->f = NULL;
            }
            if (!r) {
                r = append_fromstage(&appendstate, &body, curstage->stage,
                                     curstage->internaldate,
                                     &curstage->flags, 0,
                                     curstage->annotations);
            }
            if (body) {
                /* Note: either the calls to message_parse_binary_file()
                 * or append_fromstage() above, may create a body.  */
                message_free_body(body);
                free(body);
                body = NULL;
            }
        }

        if (!r) {
            r = append_commit(&appendstate);
        } else {
            append_abort(&appendstate);
        }
    }

    imapd_check(NULL, 1);

    if (r == IMAP_PROTOCOL_ERROR && parseerr) {
        prot_printf(imapd_out, "%s BAD %s\r\n", tag, parseerr);
    } else if (r == IMAP_BADURL) {
        prot_printf(imapd_out, "%s NO [BADURL \"%s\"] %s\r\n",
                    tag, url, parseerr);
    } else if (r) {
        prot_printf(imapd_out, "%s NO %s%s\r\n",
                    tag,
                    (r == IMAP_MAILBOX_NONEXISTENT &&
                     mboxlist_createmailboxcheck(intname, 0, 0,
                                                 imapd_userisadmin,
                                                 imapd_userid, imapd_authstate,
                                                 NULL, NULL, 0) == 0)
                    ? "[TRYCREATE] " : r == IMAP_MESSAGE_TOO_LARGE
                    ? "[TOOBIG]" : "", error_message(r));
    } else if (doappenduid) {
        /* is this a space seperated list or sequence list? */
        prot_printf(imapd_out, "%s OK [APPENDUID %lu ", tag, uidvalidity);
        if (appendstate.nummsg == 1) {
            prot_printf(imapd_out, "%u", appendstate.baseuid);
        } else {
            prot_printf(imapd_out, "%u:%u", appendstate.baseuid,
                        appendstate.baseuid + appendstate.nummsg - 1);
        }
        prot_printf(imapd_out, "] %s\r\n", error_message(IMAP_OK_COMPLETED));
    } else {
        prot_printf(imapd_out, "%s OK %s\r\n", tag,
                    error_message(IMAP_OK_COMPLETED));
    }

cleanup:
    /* Cleanup the stage(s) */
    while ((curstage = ptrarray_pop(&stages))) {
        if (curstage->f != NULL) fclose(curstage->f);
        append_removestage(curstage->stage);
        strarray_fini(&curstage->flags);
        freeentryatts(curstage->annotations);
        free(curstage);
    }
    free(intname);
    ptrarray_fini(&stages);
}
