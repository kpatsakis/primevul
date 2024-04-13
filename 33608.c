uiserver_new (void **engine, const char *file_name, const char *home_dir)
{
  gpgme_error_t err = 0;
  engine_uiserver_t uiserver;
  char *dft_display = NULL;
  char dft_ttyname[64];
  char *dft_ttytype = NULL;
  char *optstr;

  uiserver = calloc (1, sizeof *uiserver);
  if (!uiserver)
    return gpg_error_from_syserror ();

  uiserver->protocol = GPGME_PROTOCOL_DEFAULT;
  uiserver->status_cb.fd = -1;
  uiserver->status_cb.dir = 1;
  uiserver->status_cb.tag = 0;
  uiserver->status_cb.data = uiserver;

  uiserver->input_cb.fd = -1;
  uiserver->input_cb.dir = 0;
  uiserver->input_cb.tag = 0;
  uiserver->input_cb.server_fd = -1;
  *uiserver->input_cb.server_fd_str = 0;
  uiserver->output_cb.fd = -1;
  uiserver->output_cb.dir = 1;
  uiserver->output_cb.tag = 0;
  uiserver->output_cb.server_fd = -1;
  *uiserver->output_cb.server_fd_str = 0;
  uiserver->message_cb.fd = -1;
  uiserver->message_cb.dir = 0;
  uiserver->message_cb.tag = 0;
  uiserver->message_cb.server_fd = -1;
  *uiserver->message_cb.server_fd_str = 0;

  uiserver->status.fnc = 0;
  uiserver->colon.fnc = 0;
  uiserver->colon.attic.line = 0;
  uiserver->colon.attic.linesize = 0;
  uiserver->colon.attic.linelen = 0;
  uiserver->colon.any = 0;

  uiserver->inline_data = NULL;

  uiserver->io_cbs.add = NULL;
  uiserver->io_cbs.add_priv = NULL;
  uiserver->io_cbs.remove = NULL;
  uiserver->io_cbs.event = NULL;
  uiserver->io_cbs.event_priv = NULL;

  err = assuan_new_ext (&uiserver->assuan_ctx, GPG_ERR_SOURCE_GPGME,
			&_gpgme_assuan_malloc_hooks, _gpgme_assuan_log_cb,
			NULL);
  if (err)
    goto leave;
  assuan_ctx_set_system_hooks (uiserver->assuan_ctx,
			       &_gpgme_assuan_system_hooks);

  err = assuan_socket_connect (uiserver->assuan_ctx,
			       file_name ?
			       file_name : _gpgme_get_default_uisrv_socket (),
			       0, ASSUAN_SOCKET_SERVER_FDPASSING);
  if (err)
    goto leave;

  err = _gpgme_getenv ("DISPLAY", &dft_display);
  if (err)
    goto leave;
  if (dft_display)
    {
      if (asprintf (&optstr, "OPTION display=%s", dft_display) < 0)
        {
	  err = gpg_error_from_syserror ();
	  free (dft_display);
	  goto leave;
	}
      free (dft_display);

      err = assuan_transact (uiserver->assuan_ctx, optstr, NULL, NULL, NULL,
			     NULL, NULL, NULL);
      free (optstr);
      if (err)
	goto leave;
    }

  if (isatty (1))
    {
      int rc;

      rc = ttyname_r (1, dft_ttyname, sizeof (dft_ttyname));
      if (rc)
	{
	  err = gpg_error_from_errno (rc);
	  goto leave;
	}
      else
	{
	  if (asprintf (&optstr, "OPTION ttyname=%s", dft_ttyname) < 0)
	    {
	      err = gpg_error_from_syserror ();
	      goto leave;
	    }
	  err = assuan_transact (uiserver->assuan_ctx, optstr, NULL, NULL, NULL,
				 NULL, NULL, NULL);
	  free (optstr);
	  if (err)
	    goto leave;

	  err = _gpgme_getenv ("TERM", &dft_ttytype);
	  if (err)
	    goto leave;
	  if (dft_ttytype)
	    {
	      if (asprintf (&optstr, "OPTION ttytype=%s", dft_ttytype) < 0)
		{
		  err = gpg_error_from_syserror ();
		  free (dft_ttytype);
		  goto leave;
		}
	      free (dft_ttytype);

	      err = assuan_transact (uiserver->assuan_ctx, optstr, NULL, NULL,
				     NULL, NULL, NULL, NULL);
	      free (optstr);
	      if (err)
		goto leave;
	    }
	}
    }

#ifdef HAVE_W32_SYSTEM
  /* Under Windows we need to use AllowSetForegroundWindow.  Tell
     uiserver to tell us when it needs it.  */
  if (!err)
    {
      err = assuan_transact (uiserver->assuan_ctx, "OPTION allow-pinentry-notify",
                             NULL, NULL, NULL, NULL, NULL, NULL);
      if (gpg_err_code (err) == GPG_ERR_UNKNOWN_OPTION)
        err = 0; /* This is a new feature of uiserver.  */
    }
#endif /*HAVE_W32_SYSTEM*/

 leave:
  if (err)
    uiserver_release (uiserver);
  else
    *engine = uiserver;

  return err;
}
