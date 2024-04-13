static int dtls1_copy_record(SSL *s, pitem *item)
{
    DTLS1_RECORD_DATA *rdata;

    rdata = (DTLS1_RECORD_DATA *)item->data;

    SSL3_BUFFER_release(&s->rlayer.rbuf);

    s->rlayer.packet = rdata->packet;
    s->rlayer.packet_length = rdata->packet_length;
    memcpy(&s->rlayer.rbuf, &(rdata->rbuf), sizeof(SSL3_BUFFER));
    memcpy(&s->rlayer.rrec, &(rdata->rrec), sizeof(SSL3_RECORD));

    /* Set proper sequence number for mac calculation */
    memcpy(&(s->rlayer.read_sequence[2]), &(rdata->packet[5]), 6);

    return (1);
}
