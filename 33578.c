gpgsm_genkey (void *engine, gpgme_data_t help_data, int use_armor,
	      gpgme_data_t pubkey, gpgme_data_t seckey)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;

  if (!gpgsm || !pubkey || seckey)
    return gpg_error (GPG_ERR_INV_VALUE);

  gpgsm->input_cb.data = help_data;
  err = gpgsm_set_fd (gpgsm, INPUT_FD, map_data_enc (gpgsm->input_cb.data));
  if (err)
    return err;
  gpgsm->output_cb.data = pubkey;
  err = gpgsm_set_fd (gpgsm, OUTPUT_FD, use_armor ? "--armor"
		      : map_data_enc (gpgsm->output_cb.data));
  if (err)
    return err;
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, "GENKEY");
  return err;
}
