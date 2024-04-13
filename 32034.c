static void ssh2_log_incoming_packet(Ssh ssh, struct Packet *pkt)
{
    int nblanks = 0;
    struct logblank_t blanks[4];
    char *str;
    int slen;

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

    log_packet(ssh->logctx, PKT_INCOMING, pkt->type,
               ssh2_pkt_type(ssh->pkt_kctx, ssh->pkt_actx, pkt->type),
               pkt->body, pkt->length, nblanks, blanks, &pkt->sequence,
               0, NULL);
}
