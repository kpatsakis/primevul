gpgsm_sign (void *engine, gpgme_data_t in, gpgme_data_t out,
	    gpgme_sig_mode_t mode, int use_armor, int use_textmode,
	    int include_certs, gpgme_ctx_t ctx /* FIXME */)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;
  char *assuan_cmd;
  int i;
  gpgme_key_t key;

  if (!gpgsm)
    return gpg_error (GPG_ERR_INV_VALUE);

  /* FIXME: This does not work as RESET does not reset it so we can't
     revert back to default.  */
  if (include_certs != GPGME_INCLUDE_CERTS_DEFAULT)
    {
      /* FIXME: Make sure that if we run multiple operations, that we
	 can reset any previously set value in case the default is
	 requested.  */

      if (asprintf (&assuan_cmd, "OPTION include-certs %i", include_certs) < 0)
	return gpg_error_from_syserror ();
      err = gpgsm_assuan_simple_command (gpgsm->assuan_ctx, assuan_cmd,
                                         NULL, NULL);
      free (assuan_cmd);
      if (err)
	return err;
    }

  for (i = 0; (key = gpgme_signers_enum (ctx, i)); i++)
    {
      const char *s = key->subkeys ? key->subkeys->fpr : NULL;
      if (s && strlen (s) < 80)
	{
          char buf[100];

          strcpy (stpcpy (buf, "SIGNER "), s);
          err = gpgsm_assuan_simple_command (gpgsm->assuan_ctx, buf,
                                             gpgsm->status.fnc,
                                             gpgsm->status.fnc_value);
	}
      else
        err = gpg_error (GPG_ERR_INV_VALUE);
      gpgme_key_unref (key);
      if (err)
        return err;
    }

  gpgsm->input_cb.data = in;
  err = gpgsm_set_fd (gpgsm, INPUT_FD, map_data_enc (gpgsm->input_cb.data));
  if (err)
    return err;
  gpgsm->output_cb.data = out;
  err = gpgsm_set_fd (gpgsm, OUTPUT_FD, use_armor ? "--armor"
		      : map_data_enc (gpgsm->output_cb.data));
  if (err)
    return err;
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, mode == GPGME_SIG_MODE_DETACH
	       ? "SIGN --detached" : "SIGN");
  return err;
}
