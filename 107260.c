rtadv_process_solicit (struct interface *ifp)
{
  struct zebra_vrf *zvrf = vrf_info_lookup (ifp->vrf_id);

  zlog_info ("Router solicitation received on %s vrf %u", ifp->name, zvrf->vrf_id);

  rtadv_send_packet (zvrf->rtadv.sock, ifp);
}
