uiserver_set_locale (void *engine, int category, const char *value)
{
  engine_uiserver_t uiserver = engine;
  gpgme_error_t err;
  char *optstr;
  char *catstr;

  /* FIXME: If value is NULL, we need to reset the option to default.
     But we can't do this.  So we error out here.  UISERVER needs support
     for this.  */
  if (category == LC_CTYPE)
    {
      catstr = "lc-ctype";
      if (!value && uiserver->lc_ctype_set)
	return gpg_error (GPG_ERR_INV_VALUE);
      if (value)
	uiserver->lc_ctype_set = 1;
    }
#ifdef LC_MESSAGES
  else if (category == LC_MESSAGES)
    {
      catstr = "lc-messages";
      if (!value && uiserver->lc_messages_set)
	return gpg_error (GPG_ERR_INV_VALUE);
      if (value)
	uiserver->lc_messages_set = 1;
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
      err = assuan_transact (uiserver->assuan_ctx, optstr, NULL, NULL,
			     NULL, NULL, NULL, NULL);
      free (optstr);
    }

  return err;
}
