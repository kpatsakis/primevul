gpgsm_cancel (void *engine)
{
  engine_gpgsm_t gpgsm = engine;

  if (!gpgsm)
    return gpg_error (GPG_ERR_INV_VALUE);

  if (gpgsm->status_cb.fd != -1)
    _gpgme_io_close (gpgsm->status_cb.fd);
  if (gpgsm->input_cb.fd != -1)
    _gpgme_io_close (gpgsm->input_cb.fd);
  if (gpgsm->output_cb.fd != -1)
    _gpgme_io_close (gpgsm->output_cb.fd);
  if (gpgsm->message_cb.fd != -1)
    _gpgme_io_close (gpgsm->message_cb.fd);

  if (gpgsm->assuan_ctx)
    {
      assuan_release (gpgsm->assuan_ctx);
      gpgsm->assuan_ctx = NULL;
    }

  return 0;
}
