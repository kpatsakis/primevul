static struct Packet *ssh2_pkt_init(int pkt_type)
{
    struct Packet *pkt = ssh_new_packet();
    pkt->length = 5; /* space for packet length + padding length */
    pkt->forcepad = 0;
    pkt->type = pkt_type;
    ssh_pkt_addbyte(pkt, (unsigned char) pkt_type);
    pkt->body = pkt->data + pkt->length; /* after packet type */
    pkt->downstream_id = 0;
    pkt->additional_log_text = NULL;
    return pkt;
}
