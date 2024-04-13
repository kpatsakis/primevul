rtadv_event (struct zebra_vrf *zvrf, enum rtadv_event event, int val)
{
  struct rtadv *rtadv = &zvrf->rtadv;

  switch (event)
    {
    case RTADV_START:
      if (! rtadv->ra_read)
	rtadv->ra_read = thread_add_read (zebrad.master, rtadv_read, zvrf, val);
      if (! rtadv->ra_timer)
	rtadv->ra_timer = thread_add_event (zebrad.master, rtadv_timer,
	                                    zvrf, 0);
      break;
    case RTADV_STOP:
      if (rtadv->ra_timer)
	{
	  thread_cancel (rtadv->ra_timer);
	  rtadv->ra_timer = NULL;
	}
      if (rtadv->ra_read)
	{
	  thread_cancel (rtadv->ra_read);
	  rtadv->ra_read = NULL;
	}
      break;
    case RTADV_TIMER:
      if (! rtadv->ra_timer)
	rtadv->ra_timer = thread_add_timer (zebrad.master, rtadv_timer, zvrf,
	                                    val);
      break;
    case RTADV_TIMER_MSEC:
      if (! rtadv->ra_timer)
	rtadv->ra_timer = thread_add_timer_msec (zebrad.master, rtadv_timer, 
					    zvrf, val);
      break;
    case RTADV_READ:
      if (! rtadv->ra_read)
	rtadv->ra_read = thread_add_read (zebrad.master, rtadv_read, zvrf, val);
      break;
    default:
      break;
    }
  return;
}
