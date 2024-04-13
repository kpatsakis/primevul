add_io_cb (engine_uiserver_t uiserver, iocb_data_t *iocbd, gpgme_io_cb_t handler)
{
  gpgme_error_t err;

  TRACE_BEG2 (DEBUG_ENGINE, "engine-uiserver:add_io_cb", uiserver,
              "fd %d, dir %d", iocbd->fd, iocbd->dir);
  err = (*uiserver->io_cbs.add) (uiserver->io_cbs.add_priv,
			      iocbd->fd, iocbd->dir,
			      handler, iocbd->data, &iocbd->tag);
  if (err)
    return TRACE_ERR (err);
  if (!iocbd->dir)
    /* FIXME Kludge around poll() problem.  */
    err = _gpgme_io_set_nonblocking (iocbd->fd);
  return TRACE_ERR (err);
}
