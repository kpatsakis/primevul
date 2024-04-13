start (engine_uiserver_t uiserver, const char *command)
{
  gpgme_error_t err;
  int fdlist[5];
  int nfds;

  /* We need to know the fd used by assuan for reads.  We do this by
     using the assumption that the first returned fd from
     assuan_get_active_fds() is always this one.  */
  nfds = assuan_get_active_fds (uiserver->assuan_ctx, 0 /* read fds */,
                                fdlist, DIM (fdlist));
  if (nfds < 1)
    return gpg_error (GPG_ERR_GENERAL);	/* FIXME */

  /* We "duplicate" the file descriptor, so we can close it here (we
     can't close fdlist[0], as that is closed by libassuan, and
     closing it here might cause libassuan to close some unrelated FD
     later).  Alternatively, we could special case status_fd and
     register/unregister it manually as needed, but this increases
     code duplication and is more complicated as we can not use the
     close notifications etc.  A third alternative would be to let
     Assuan know that we closed the FD, but that complicates the
     Assuan interface.  */

  uiserver->status_cb.fd = _gpgme_io_dup (fdlist[0]);
  if (uiserver->status_cb.fd < 0)
    return gpg_error_from_syserror ();

  if (_gpgme_io_set_close_notify (uiserver->status_cb.fd,
				  close_notify_handler, uiserver))
    {
      _gpgme_io_close (uiserver->status_cb.fd);
      uiserver->status_cb.fd = -1;
      return gpg_error (GPG_ERR_GENERAL);
    }

  err = add_io_cb (uiserver, &uiserver->status_cb, status_handler);
  if (!err && uiserver->input_cb.fd != -1)
    err = add_io_cb (uiserver, &uiserver->input_cb, _gpgme_data_outbound_handler);
  if (!err && uiserver->output_cb.fd != -1)
    err = add_io_cb (uiserver, &uiserver->output_cb, _gpgme_data_inbound_handler);
  if (!err && uiserver->message_cb.fd != -1)
    err = add_io_cb (uiserver, &uiserver->message_cb, _gpgme_data_outbound_handler);

  if (!err)
    err = assuan_write_line (uiserver->assuan_ctx, command);

  if (!err)
    uiserver_io_event (uiserver, GPGME_EVENT_START, NULL);

  return err;
}
