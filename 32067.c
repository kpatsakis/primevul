static void ssh_pkt_addstring_str(struct Packet *pkt, const char *data)
{
  ssh_pkt_addstring_data(pkt, data, strlen(data));
}
