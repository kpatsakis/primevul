static struct Packet *ssh2_bare_connection_rdpkt(Ssh ssh,
                                                 const unsigned char **data,
                                                 int *datalen)
{
    struct rdpkt2_bare_state_tag *st = &ssh->rdpkt2_bare_state;

    crBegin(ssh->ssh2_bare_rdpkt_crstate);

    /*
     * Read the packet length field.
     */
    for (st->i = 0; st->i < 4; st->i++) {
        while ((*datalen) == 0)
            crReturn(NULL);
        st->length[st->i] = *(*data)++;
        (*datalen)--;
    }

    st->packetlen = toint(GET_32BIT_MSB_FIRST(st->length));
    if (st->packetlen <= 0 || st->packetlen >= OUR_V2_PACKETLIMIT) {
        bombout(("Invalid packet length received"));
        crStop(NULL);
    }

    st->pktin = ssh_new_packet();
    st->pktin->data = snewn(st->packetlen, unsigned char);

    st->pktin->encrypted_len = st->packetlen;

    st->pktin->sequence = st->incoming_sequence++;

    /*
     * Read the remainder of the packet.
     */
    for (st->i = 0; st->i < st->packetlen; st->i++) {
        while ((*datalen) == 0)
            crReturn(NULL);
        st->pktin->data[st->i] = *(*data)++;
        (*datalen)--;
    }

    /*
     * pktin->body and pktin->length should identify the semantic
     * content of the packet, excluding the initial type byte.
     */
    st->pktin->type = st->pktin->data[0];
    st->pktin->body = st->pktin->data + 1;
    st->pktin->length = st->packetlen - 1;

    /*
     * Log incoming packet, possibly omitting sensitive fields.
     */
    if (ssh->logctx)
        ssh2_log_incoming_packet(ssh, st->pktin);

    st->pktin->savedpos = 0;

    crFinish(st->pktin);
}
