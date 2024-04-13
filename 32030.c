static struct Packet *ssh1_pkt_init(int pkt_type)
{
    struct Packet *pkt = ssh_new_packet();
    pkt->length = 4 + 8;	    /* space for length + max padding */
    ssh_pkt_addbyte(pkt, pkt_type);
    pkt->body = pkt->data + pkt->length;
    pkt->type = pkt_type;
    pkt->downstream_id = 0;
    pkt->additional_log_text = NULL;
    return pkt;
}
