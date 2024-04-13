static void ssh1_log_incoming_packet(Ssh ssh, struct Packet *pkt)
{
    int nblanks = 0;
    struct logblank_t blanks[4];
    char *str;
    int slen;

    pkt->savedpos = 0;

    if (ssh->logomitdata &&
        (pkt->type == SSH1_SMSG_STDOUT_DATA ||
         pkt->type == SSH1_SMSG_STDERR_DATA ||
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
    log_packet(ssh->logctx, PKT_INCOMING, pkt->type,
               ssh1_pkt_type(pkt->type),
               pkt->body, pkt->length, nblanks, blanks, NULL,
               0, NULL);
}
