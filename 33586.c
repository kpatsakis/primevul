gpgsm_passwd (void *engine, gpgme_key_t key, unsigned int flags)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;
  char *line;

  if (!key || !key->subkeys || !key->subkeys->fpr)
    return gpg_error (GPG_ERR_INV_CERT_OBJ);

  if (asprintf (&line, "PASSWD -- %s", key->subkeys->fpr) < 0)
    return gpg_error_from_syserror ();

  gpgsm_clear_fd (gpgsm, OUTPUT_FD);
  gpgsm_clear_fd (gpgsm, INPUT_FD);
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, line);
  free (line);

  return err;
}
