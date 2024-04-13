rtadv_config_write (struct vty *vty, struct interface *ifp)
{
  struct zebra_if *zif;
  struct listnode *node;
  struct rtadv_prefix *rprefix;
  char buf[PREFIX_STRLEN];
  int interval;

  zif = ifp->info;

  if (! if_is_loopback (ifp))
    {
      if (zif->rtadv.AdvSendAdvertisements)
	vty_out (vty, " no ipv6 nd suppress-ra%s", VTY_NEWLINE);
    }

  
  interval = zif->rtadv.MaxRtrAdvInterval;
  if (interval % 1000)
    vty_out (vty, " ipv6 nd ra-interval msec %d%s", interval,
	     VTY_NEWLINE);
  else
    if (interval != RTADV_MAX_RTR_ADV_INTERVAL)
      vty_out (vty, " ipv6 nd ra-interval %d%s", interval / 1000,
	     VTY_NEWLINE);

  if (zif->rtadv.AdvIntervalOption)
    vty_out (vty, " ipv6 nd adv-interval-option%s", VTY_NEWLINE);

  if (zif->rtadv.AdvDefaultLifetime != -1)
    vty_out (vty, " ipv6 nd ra-lifetime %d%s", zif->rtadv.AdvDefaultLifetime,
	     VTY_NEWLINE);

  if (zif->rtadv.HomeAgentPreference)
    vty_out (vty, " ipv6 nd home-agent-preference %u%s",
	     zif->rtadv.HomeAgentPreference, VTY_NEWLINE);

  if (zif->rtadv.HomeAgentLifetime != -1)
    vty_out (vty, " ipv6 nd home-agent-lifetime %u%s",
	     zif->rtadv.HomeAgentLifetime, VTY_NEWLINE);

  if (zif->rtadv.AdvHomeAgentFlag)
    vty_out (vty, " ipv6 nd home-agent-config-flag%s", VTY_NEWLINE);

  if (zif->rtadv.AdvReachableTime)
    vty_out (vty, " ipv6 nd reachable-time %d%s", zif->rtadv.AdvReachableTime,
	     VTY_NEWLINE);

  if (zif->rtadv.AdvManagedFlag)
    vty_out (vty, " ipv6 nd managed-config-flag%s", VTY_NEWLINE);

  if (zif->rtadv.AdvOtherConfigFlag)
    vty_out (vty, " ipv6 nd other-config-flag%s", VTY_NEWLINE);

  if (zif->rtadv.DefaultPreference != RTADV_PREF_MEDIUM)
    vty_out (vty, " ipv6 nd router-preference %s%s",
	     rtadv_pref_strs[zif->rtadv.DefaultPreference],
	     VTY_NEWLINE);

  if (zif->rtadv.AdvLinkMTU)
    vty_out (vty, " ipv6 nd mtu %d%s", zif->rtadv.AdvLinkMTU, VTY_NEWLINE);

  for (ALL_LIST_ELEMENTS_RO (zif->rtadv.AdvPrefixList, node, rprefix))
    {
      vty_out (vty, " ipv6 nd prefix %s",
               prefix2str (&rprefix->prefix, buf, sizeof(buf)));
      if ((rprefix->AdvValidLifetime != RTADV_VALID_LIFETIME) || 
	  (rprefix->AdvPreferredLifetime != RTADV_PREFERRED_LIFETIME))
	{
	  if (rprefix->AdvValidLifetime == UINT32_MAX)
	    vty_out (vty, " infinite");
	  else
	    vty_out (vty, " %u", rprefix->AdvValidLifetime);
	  if (rprefix->AdvPreferredLifetime == UINT32_MAX)
	    vty_out (vty, " infinite");
	  else
	    vty_out (vty, " %u", rprefix->AdvPreferredLifetime);
	}
      if (!rprefix->AdvOnLinkFlag)
	vty_out (vty, " off-link");
      if (!rprefix->AdvAutonomousFlag)
	vty_out (vty, " no-autoconfig");
      if (rprefix->AdvRouterAddressFlag)
	vty_out (vty, " router-address");
      vty_out (vty, "%s", VTY_NEWLINE);
    }
}
