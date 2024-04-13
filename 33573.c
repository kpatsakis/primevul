gpgsm_decrypt (void *engine, gpgme_data_t ciph, gpgme_data_t plain)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;

  if (!gpgsm)
    return gpg_error (GPG_ERR_INV_VALUE);

  gpgsm->input_cb.data = ciph;
  err = gpgsm_set_fd (gpgsm, INPUT_FD, map_data_enc (gpgsm->input_cb.data));
  if (err)
    return gpg_error (GPG_ERR_GENERAL);	/* FIXME */
  gpgsm->output_cb.data = plain;
  err = gpgsm_set_fd (gpgsm, OUTPUT_FD, 0);
  if (err)
    return gpg_error (GPG_ERR_GENERAL);	/* FIXME */
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (engine, "DECRYPT");
  return err;
}
