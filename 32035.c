static void ssh2_log_outgoing_packet(Ssh ssh, struct Packet *pkt)
{
    int nblanks = 0;
    struct logblank_t blanks[4];
    char *str;
    int slen;

    /*
     * For outgoing packets, pkt->length represents the length of the
     * whole packet starting at pkt->data (including some header), and
     * pkt->body refers to the point within that where the log-worthy
     * payload begins. However, incoming packets expect pkt->length to
     * represent only the payload length (that is, it's measured from
     * pkt->body not from pkt->data). Temporarily adjust our outgoing
     * packet to conform to the incoming-packet semantics, so that we
     * can analyse it with the ssh_pkt_get functions.
     */
    pkt->length -= (pkt->body - pkt->data);
    pkt->savedpos = 0;

    if (ssh->logomitdata &&
        (pkt->type == SSH2_MSG_CHANNEL_DATA ||
         pkt->type == SSH2_MSG_CHANNEL_EXTENDED_DATA)) {
        /* "Session data" packets - omit the data string. */
        ssh_pkt_getuint32(pkt);    /* skip channel id */
        if (pkt->type == SSH2_MSG_CHANNEL_EXTENDED_DATA)
            ssh_pkt_getuint32(pkt);    /* skip extended data type */
        blanks[nblanks].offset = pkt->savedpos + 4;
        blanks[nblanks].type = PKTLOG_OMIT;
        ssh_pkt_getstring(pkt, &str, &slen);
        if (str) {
            blanks[nblanks].len = slen;
            nblanks++;
        }
    }

    if (pkt->type == SSH2_MSG_USERAUTH_REQUEST &&
        conf_get_int(ssh->conf, CONF_logomitpass)) {
        /* If this is a password packet, blank the password(s). */
        pkt->savedpos = 0;
        ssh_pkt_getstring(pkt, &str, &slen);
        ssh_pkt_getstring(pkt, &str, &slen);
        ssh_pkt_getstring(pkt, &str, &slen);
        if (slen == 8 && !memcmp(str, "password", 8)) {
            ssh2_pkt_getbool(pkt);
            /* Blank the password field. */
            blanks[nblanks].offset = pkt->savedpos;
            blanks[nblanks].type = PKTLOG_BLANK;
            ssh_pkt_getstring(pkt, &str, &slen);
            if (str) {
                blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
                nblanks++;
                /* If there's another password field beyond it (change of
                 * password), blank that too. */
                ssh_pkt_getstring(pkt, &str, &slen);
                if (str)
                    blanks[nblanks-1].len =
                        pkt->savedpos - blanks[nblanks].offset;
            }
        }
    } else if (ssh->pkt_actx == SSH2_PKTCTX_KBDINTER &&
               pkt->type == SSH2_MSG_USERAUTH_INFO_RESPONSE &&
               conf_get_int(ssh->conf, CONF_logomitpass)) {
        /* If this is a keyboard-interactive response packet, blank
         * the responses. */
        pkt->savedpos = 0;
        ssh_pkt_getuint32(pkt);
        blanks[nblanks].offset = pkt->savedpos;
        blanks[nblanks].type = PKTLOG_BLANK;
        while (1) {
            ssh_pkt_getstring(pkt, &str, &slen);
            if (!str)
                break;
        }
        blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
        nblanks++;
    } else if (pkt->type == SSH2_MSG_CHANNEL_REQUEST &&
               conf_get_int(ssh->conf, CONF_logomitpass)) {
        /*
         * If this is an X forwarding request packet, blank the fake
         * auth data.
         *
         * Note that while we blank the X authentication data here, we
         * don't take any special action to blank the start of an X11
         * channel, so using MIT-MAGIC-COOKIE-1 and actually opening
         * an X connection without having session blanking enabled is
         * likely to leak your cookie into the log.
         */
        pkt->savedpos = 0;
        ssh_pkt_getuint32(pkt);
        ssh_pkt_getstring(pkt, &str, &slen);
        if (slen == 7 && !memcmp(str, "x11-req", 0)) {
            ssh2_pkt_getbool(pkt);
            ssh2_pkt_getbool(pkt);
            ssh_pkt_getstring(pkt, &str, &slen);
            blanks[nblanks].offset = pkt->savedpos;
            blanks[nblanks].type = PKTLOG_BLANK;
            ssh_pkt_getstring(pkt, &str, &slen);
            if (str) {
                blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
                nblanks++;
            }
        }
    }

    log_packet(ssh->logctx, PKT_OUTGOING, pkt->data[5],
               ssh2_pkt_type(ssh->pkt_kctx, ssh->pkt_actx, pkt->data[5]),
               pkt->body, pkt->length, nblanks, blanks,
               &ssh->v2_outgoing_sequence,
               pkt->downstream_id, pkt->additional_log_text);

    /*
     * Undo the above adjustment of pkt->length, to put the packet
     * back in the state we found it.
     */
    pkt->length += (pkt->body - pkt->data);
}
