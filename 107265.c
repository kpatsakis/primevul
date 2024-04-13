rtadv_timer (struct thread *thread)
{
  struct zebra_vrf *zvrf = THREAD_ARG (thread);
  struct listnode *node, *nnode;
  struct interface *ifp;
  struct zebra_if *zif;
  int period;

  zvrf->rtadv.ra_timer = NULL;
  if (zvrf->rtadv.adv_msec_if_count == 0)
    {
      period = 1000; /* 1 s */
      rtadv_event (zvrf, RTADV_TIMER, 1 /* 1 s */);
    } 
  else
    {
      period = 10; /* 10 ms */
      rtadv_event (zvrf, RTADV_TIMER_MSEC, 10 /* 10 ms */);
    }

  for (ALL_LIST_ELEMENTS (vrf_iflist (zvrf->vrf_id), node, nnode, ifp))
    {
      if (if_is_loopback (ifp) || ! if_is_operative (ifp))
	continue;

      zif = ifp->info;

      if (zif->rtadv.AdvSendAdvertisements)
	{ 
	  zif->rtadv.AdvIntervalTimer -= period;
	  if (zif->rtadv.AdvIntervalTimer <= 0)
	    {
	      /* FIXME: using MaxRtrAdvInterval each time isn't what section
	         6.2.4 of RFC4861 tells to do. */
	      zif->rtadv.AdvIntervalTimer = zif->rtadv.MaxRtrAdvInterval;
	      rtadv_send_packet (zvrf->rtadv.sock, ifp);
	    }
	}
    }
  return 0;
}
