default_inq_cb (engine_gpgsm_t gpgsm, const char *line)
{
  if (!strncmp (line, "PINENTRY_LAUNCHED", 17) && (line[17]==' '||!line[17]))
    {
      _gpgme_allow_set_foreground_window ((pid_t)strtoul (line+17, NULL, 10));
    }

  return 0;
}
