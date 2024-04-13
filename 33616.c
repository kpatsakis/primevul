uiserver_verify (void *engine, gpgme_data_t sig, gpgme_data_t signed_text,
	      gpgme_data_t plaintext)
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

  if (asprintf (&cmd, "VERIFY%s", protocol) < 0)
    return gpg_error_from_syserror ();

  uiserver->input_cb.data = sig;
  err = uiserver_set_fd (uiserver, INPUT_FD,
			 map_data_enc (uiserver->input_cb.data));
  if (err)
    {
      free (cmd);
      return err;
    }
  if (plaintext)
    {
      /* Normal or cleartext signature.  */
      uiserver->output_cb.data = plaintext;
      err = uiserver_set_fd (uiserver, OUTPUT_FD, 0);
    }
  else
    {
      /* Detached signature.  */
      uiserver->message_cb.data = signed_text;
      err = uiserver_set_fd (uiserver, MESSAGE_FD, 0);
    }
  uiserver->inline_data = NULL;

  if (!err)
    err = start (uiserver, cmd);

  free (cmd);
  return err;
}
