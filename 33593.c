gpgsm_verify (void *engine, gpgme_data_t sig, gpgme_data_t signed_text,
	      gpgme_data_t plaintext)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;

  if (!gpgsm)
    return gpg_error (GPG_ERR_INV_VALUE);

  gpgsm->input_cb.data = sig;
  err = gpgsm_set_fd (gpgsm, INPUT_FD, map_data_enc (gpgsm->input_cb.data));
  if (err)
    return err;
  if (plaintext)
    {
      /* Normal or cleartext signature.  */
      gpgsm->output_cb.data = plaintext;
      err = gpgsm_set_fd (gpgsm, OUTPUT_FD, 0);
      gpgsm_clear_fd (gpgsm, MESSAGE_FD);
    }
  else
    {
      /* Detached signature.  */
      gpgsm->message_cb.data = signed_text;
      err = gpgsm_set_fd (gpgsm, MESSAGE_FD, 0);
      gpgsm_clear_fd (gpgsm, OUTPUT_FD);
    }
  gpgsm->inline_data = NULL;

  if (!err)
    err = start (gpgsm, "VERIFY");

  return err;
}
