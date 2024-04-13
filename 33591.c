gpgsm_set_locale (void *engine, int category, const char *value)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;
  char *optstr;
  char *catstr;

  /* FIXME: If value is NULL, we need to reset the option to default.
     But we can't do this.  So we error out here.  GPGSM needs support
     for this.  */
  if (0)
    ;
#ifdef LC_CTYPE
  else if (category == LC_CTYPE)
    {
      catstr = "lc-ctype";
      if (!value && gpgsm->lc_ctype_set)
	return gpg_error (GPG_ERR_INV_VALUE);
      if (value)
	gpgsm->lc_ctype_set = 1;
    }
#endif
#ifdef LC_MESSAGES
  else if (category == LC_MESSAGES)
    {
      catstr = "lc-messages";
      if (!value && gpgsm->lc_messages_set)
	return gpg_error (GPG_ERR_INV_VALUE);
      if (value)
	gpgsm->lc_messages_set = 1;
    }
#endif /* LC_MESSAGES */
  else
    return gpg_error (GPG_ERR_INV_VALUE);

  /* FIXME: Reset value to default.  */
  if (!value)
    return 0;

  if (asprintf (&optstr, "OPTION %s=%s", catstr, value) < 0)
    err = gpg_error_from_syserror ();
  else
    {
      err = assuan_transact (gpgsm->assuan_ctx, optstr, NULL, NULL,
			     NULL, NULL, NULL, NULL);
      free (optstr);
    }

  return err;
}
