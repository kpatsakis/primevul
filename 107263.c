rtadv_terminate (struct zebra_vrf *zvrf)
{
  rtadv_event (zvrf, RTADV_STOP, 0);

  if (zvrf->rtadv.sock >= 0)
    {
      close (zvrf->rtadv.sock);
      zvrf->rtadv.sock = -1;
    }

  zvrf->rtadv.adv_if_count = 0;
  zvrf->rtadv.adv_msec_if_count = 0;
}
