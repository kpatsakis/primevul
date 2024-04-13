static void ssh2_pkt_addmp(struct Packet *pkt, Bignum b)
{
    unsigned char *p;
    int len;
    p = ssh2_mpint_fmt(b, &len);
    ssh_pkt_addstring_start(pkt);
    ssh_pkt_addstring_data(pkt, (char *)p, len);
    sfree(p);
}
