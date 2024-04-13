sd_source_new (void)
{
  GSource *source;
  SdSource *sd_source;
  int ret;

  source = g_source_new (&sd_source_funcs, sizeof (SdSource));
  sd_source = (SdSource *)source;

  if ((ret = sd_login_monitor_new (NULL, &sd_source->monitor)) < 0)
    {
      g_printerr ("Error getting login monitor: %d", ret);
    }
  else
    {
      sd_source->pollfd.fd = sd_login_monitor_get_fd (sd_source->monitor);
      sd_source->pollfd.events = G_IO_IN;
      g_source_add_poll (source, &sd_source->pollfd);
    }

  return source;
}
