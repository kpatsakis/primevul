gpgsm_encrypt (void *engine, gpgme_key_t recp[], gpgme_encrypt_flags_t flags,
	       gpgme_data_t plain, gpgme_data_t ciph, int use_armor)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;

  if (!gpgsm)
    return gpg_error (GPG_ERR_INV_VALUE);
  if (!recp)
    return gpg_error (GPG_ERR_NOT_IMPLEMENTED);

  if (flags & GPGME_ENCRYPT_NO_ENCRYPT_TO)
    {
      err = gpgsm_assuan_simple_command (gpgsm->assuan_ctx,
					 "OPTION no-encrypt-to", NULL, NULL);
      if (err)
	return err;
    }

  gpgsm->input_cb.data = plain;
  err = gpgsm_set_fd (gpgsm, INPUT_FD, map_data_enc (gpgsm->input_cb.data));
  if (err)
    return err;
  gpgsm->output_cb.data = ciph;
  err = gpgsm_set_fd (gpgsm, OUTPUT_FD, use_armor ? "--armor"
		      : map_data_enc (gpgsm->output_cb.data));
  if (err)
    return err;
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = set_recipients (gpgsm, recp);

  if (!err)
    err = start (gpgsm, "ENCRYPT");

  return err;
}
