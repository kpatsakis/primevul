static int tls1_alpn_handle_client_hello(SSL *s, PACKET *pkt, int *al)
{
    PACKET protocol_list, save_protocol_list, protocol;

    *al = SSL_AD_DECODE_ERROR;

    if (!PACKET_as_length_prefixed_2(pkt, &protocol_list)
        || PACKET_remaining(&protocol_list) < 2) {
        return 0;
    }

    save_protocol_list = protocol_list;
    do {
        /* Protocol names can't be empty. */
        if (!PACKET_get_length_prefixed_1(&protocol_list, &protocol)
            || PACKET_remaining(&protocol) == 0) {
            return 0;
        }
    } while (PACKET_remaining(&protocol_list) != 0);

    if (!PACKET_memdup(&save_protocol_list,
                       &s->s3->alpn_proposed, &s->s3->alpn_proposed_len)) {
        *al = TLS1_AD_INTERNAL_ERROR;
        return 0;
    }

    return 1;
}
