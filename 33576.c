gpgsm_export (void *engine, const char *pattern, gpgme_export_mode_t mode,
	      gpgme_data_t keydata, int use_armor)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err = 0;
  char *cmd;

  if (!gpgsm)
    return gpg_error (GPG_ERR_INV_VALUE);

  if (mode)
    return gpg_error (GPG_ERR_NOT_SUPPORTED);

  if (!pattern)
    pattern = "";

  cmd = malloc (7 + strlen (pattern) + 1);
  if (!cmd)
    return gpg_error_from_syserror ();
  strcpy (cmd, "EXPORT ");
  strcpy (&cmd[7], pattern);

  gpgsm->output_cb.data = keydata;
  err = gpgsm_set_fd (gpgsm, OUTPUT_FD, use_armor ? "--armor"
		      : map_data_enc (gpgsm->output_cb.data));
  if (err)
    return err;
  gpgsm_clear_fd (gpgsm, INPUT_FD);
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, cmd);
  free (cmd);
  return err;
}
