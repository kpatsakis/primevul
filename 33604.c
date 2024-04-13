uiserver_encrypt (void *engine, gpgme_key_t recp[], gpgme_encrypt_flags_t flags,
		  gpgme_data_t plain, gpgme_data_t ciph, int use_armor)
{
  engine_uiserver_t uiserver = engine;
  gpgme_error_t err;
  const char *protocol;
  char *cmd;

  if (!uiserver)
    return gpg_error (GPG_ERR_INV_VALUE);
  if (uiserver->protocol == GPGME_PROTOCOL_DEFAULT)
    protocol = "";
  else if (uiserver->protocol == GPGME_PROTOCOL_OpenPGP)
    protocol = " --protocol=OpenPGP";
  else if (uiserver->protocol == GPGME_PROTOCOL_CMS)
    protocol = " --protocol=CMS";
  else
    return gpgme_error (GPG_ERR_UNSUPPORTED_PROTOCOL);

  if (flags & GPGME_ENCRYPT_PREPARE)
    {
      if (!recp || plain || ciph)
	return gpg_error (GPG_ERR_INV_VALUE);

      if (asprintf (&cmd, "PREP_ENCRYPT%s%s", protocol,
		    (flags & GPGME_ENCRYPT_EXPECT_SIGN)
		    ? " --expect-sign" : "") < 0)
	return gpg_error_from_syserror ();
    }
  else
    {
      if (!plain || !ciph)
	return gpg_error (GPG_ERR_INV_VALUE);

      if (asprintf (&cmd, "ENCRYPT%s", protocol) < 0)
	return gpg_error_from_syserror ();
    }

  if (plain)
    {
      uiserver->input_cb.data = plain;
      err = uiserver_set_fd (uiserver, INPUT_FD,
			     map_data_enc (uiserver->input_cb.data));
      if (err)
	{
	  free (cmd);
	  return err;
	}
    }

  if (ciph)
    {
      uiserver->output_cb.data = ciph;
      err = uiserver_set_fd (uiserver, OUTPUT_FD, use_armor ? "--armor"
			     : map_data_enc (uiserver->output_cb.data));
      if (err)
	{
	  free (cmd);
	  return err;
	}
    }

  uiserver->inline_data = NULL;

  if (recp)
    {
      err = set_recipients (uiserver, recp);
      if (err)
	{
	  free (cmd);
	  return err;
	}
    }

  err = start (uiserver, cmd);
  free (cmd);
  return err;
}
