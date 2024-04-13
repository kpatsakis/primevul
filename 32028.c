static void ssh1_log_outgoing_packet(Ssh ssh, struct Packet *pkt)
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
        (pkt->type == SSH1_CMSG_STDIN_DATA ||
         pkt->type == SSH1_MSG_CHANNEL_DATA)) {
        /* "Session data" packets - omit the data string. */
        if (pkt->type == SSH1_MSG_CHANNEL_DATA)
            ssh_pkt_getuint32(pkt);    /* skip channel id */
        blanks[nblanks].offset = pkt->savedpos + 4;
        blanks[nblanks].type = PKTLOG_OMIT;
        ssh_pkt_getstring(pkt, &str, &slen);
        if (str) {
            blanks[nblanks].len = slen;
            nblanks++;
        }
    }

    if ((pkt->type == SSH1_CMSG_AUTH_PASSWORD ||
         pkt->type == SSH1_CMSG_AUTH_TIS_RESPONSE ||
         pkt->type == SSH1_CMSG_AUTH_CCARD_RESPONSE) &&
        conf_get_int(ssh->conf, CONF_logomitpass)) {
        /* If this is a password or similar packet, blank the password(s). */
        blanks[nblanks].offset = 0;
        blanks[nblanks].len = pkt->length;
        blanks[nblanks].type = PKTLOG_BLANK;
        nblanks++;
    } else if (pkt->type == SSH1_CMSG_X11_REQUEST_FORWARDING &&
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
        ssh_pkt_getstring(pkt, &str, &slen);
        blanks[nblanks].offset = pkt->savedpos;
        blanks[nblanks].type = PKTLOG_BLANK;
        ssh_pkt_getstring(pkt, &str, &slen);
        if (str) {
            blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
            nblanks++;
        }
    }

    log_packet(ssh->logctx, PKT_OUTGOING, pkt->data[12],
               ssh1_pkt_type(pkt->data[12]),
               pkt->body, pkt->length,
               nblanks, blanks, NULL, 0, NULL);

    /*
     * Undo the above adjustment of pkt->length, to put the packet
     * back in the state we found it.
     */
    pkt->length += (pkt->body - pkt->data);
}
