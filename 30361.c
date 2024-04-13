sleep_between_retrievals (int count)
{
  static bool first_retrieval = true;

  if (first_retrieval)
    {
      /* Don't sleep before the very first retrieval. */
      first_retrieval = false;
      return;
    }

  if (opt.waitretry && count > 1)
    {
      /* If opt.waitretry is specified and this is a retry, wait for
         COUNT-1 number of seconds, or for opt.waitretry seconds.  */
      if (count <= opt.waitretry)
        xsleep (count - 1);
      else
        xsleep (opt.waitretry);
    }
  else if (opt.wait)
    {
      if (!opt.random_wait || count > 1)
        /* If random-wait is not specified, or if we are sleeping
           between retries of the same download, sleep the fixed
           interval.  */
        xsleep (opt.wait);
      else
        {
          /* Sleep a random amount of time averaging in opt.wait
             seconds.  The sleeping amount ranges from 0.5*opt.wait to
             1.5*opt.wait.  */
          double waitsecs = (0.5 + random_float ()) * opt.wait;
          DEBUGP (("sleep_between_retrievals: avg=%f,sleep=%f\n",
                   opt.wait, waitsecs));
          xsleep (waitsecs);
        }
    }
}
