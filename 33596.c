_uiserver_decrypt (void *engine, int verify,
		   gpgme_data_t ciph, gpgme_data_t plain)
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

  if (asprintf (&cmd, "DECRYPT%s%s", protocol,
		verify ? "" : " --no-verify") < 0)
    return gpg_error_from_syserror ();

  uiserver->input_cb.data = ciph;
  err = uiserver_set_fd (uiserver, INPUT_FD,
			 map_data_enc (uiserver->input_cb.data));
  if (err)
    {
      free (cmd);
      return gpg_error (GPG_ERR_GENERAL);	/* FIXME */
    }
  uiserver->output_cb.data = plain;
  err = uiserver_set_fd (uiserver, OUTPUT_FD, 0);
  if (err)
    {
      free (cmd);
      return gpg_error (GPG_ERR_GENERAL);	/* FIXME */
    }
  uiserver->inline_data = NULL;

  err = start (engine, cmd);
  free (cmd);
  return err;
}
