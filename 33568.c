add_io_cb (engine_gpgsm_t gpgsm, iocb_data_t *iocbd, gpgme_io_cb_t handler)
{
  gpgme_error_t err;

  TRACE_BEG2 (DEBUG_ENGINE, "engine-gpgsm:add_io_cb", gpgsm,
              "fd %d, dir %d", iocbd->fd, iocbd->dir);
  err = (*gpgsm->io_cbs.add) (gpgsm->io_cbs.add_priv,
			      iocbd->fd, iocbd->dir,
			      handler, iocbd->data, &iocbd->tag);
  if (err)
    return TRACE_ERR (err);
  if (!iocbd->dir)
    /* FIXME Kludge around poll() problem.  */
    err = _gpgme_io_set_nonblocking (iocbd->fd);
  return TRACE_ERR (err);
}
