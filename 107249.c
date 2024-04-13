rtadv_init (struct zebra_vrf *zvrf)
{
  zvrf->rtadv.sock = rtadv_make_socket (zvrf->vrf_id);
}
