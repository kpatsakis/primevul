gpgsm_new (void **engine, const char *file_name, const char *home_dir)
{
  gpgme_error_t err = 0;
  engine_gpgsm_t gpgsm;
  const char *pgmname;
  const char *argv[5];
  int argc;
#if !USE_DESCRIPTOR_PASSING
  int fds[2];
  int child_fds[4];
#endif
  char *dft_display = NULL;
  char dft_ttyname[64];
  char *dft_ttytype = NULL;
  char *optstr;

  gpgsm = calloc (1, sizeof *gpgsm);
  if (!gpgsm)
    return gpg_error_from_syserror ();

  gpgsm->status_cb.fd = -1;
  gpgsm->status_cb.dir = 1;
  gpgsm->status_cb.tag = 0;
  gpgsm->status_cb.data = gpgsm;

  gpgsm->input_cb.fd = -1;
  gpgsm->input_cb.dir = 0;
  gpgsm->input_cb.tag = 0;
  gpgsm->input_cb.server_fd = -1;
  *gpgsm->input_cb.server_fd_str = 0;
  gpgsm->output_cb.fd = -1;
  gpgsm->output_cb.dir = 1;
  gpgsm->output_cb.tag = 0;
  gpgsm->output_cb.server_fd = -1;
  *gpgsm->output_cb.server_fd_str = 0;
  gpgsm->message_cb.fd = -1;
  gpgsm->message_cb.dir = 0;
  gpgsm->message_cb.tag = 0;
  gpgsm->message_cb.server_fd = -1;
  *gpgsm->message_cb.server_fd_str = 0;

  gpgsm->status.fnc = 0;
  gpgsm->colon.fnc = 0;
  gpgsm->colon.attic.line = 0;
  gpgsm->colon.attic.linesize = 0;
  gpgsm->colon.attic.linelen = 0;
  gpgsm->colon.any = 0;

  gpgsm->inline_data = NULL;

  gpgsm->io_cbs.add = NULL;
  gpgsm->io_cbs.add_priv = NULL;
  gpgsm->io_cbs.remove = NULL;
  gpgsm->io_cbs.event = NULL;
  gpgsm->io_cbs.event_priv = NULL;

#if !USE_DESCRIPTOR_PASSING
  if (_gpgme_io_pipe (fds, 0) < 0)
    {
      err = gpg_error_from_syserror ();
      goto leave;
    }
  gpgsm->input_cb.fd = fds[1];
  gpgsm->input_cb.server_fd = fds[0];

  if (_gpgme_io_pipe (fds, 1) < 0)
    {
      err = gpg_error_from_syserror ();
      goto leave;
    }
  gpgsm->output_cb.fd = fds[0];
  gpgsm->output_cb.server_fd = fds[1];

  if (_gpgme_io_pipe (fds, 0) < 0)
    {
      err = gpg_error_from_syserror ();
      goto leave;
    }
  gpgsm->message_cb.fd = fds[1];
  gpgsm->message_cb.server_fd = fds[0];

  child_fds[0] = gpgsm->input_cb.server_fd;
  child_fds[1] = gpgsm->output_cb.server_fd;
  child_fds[2] = gpgsm->message_cb.server_fd;
  child_fds[3] = -1;
#endif

  pgmname = file_name ? file_name : _gpgme_get_default_gpgsm_name ();

  argc = 0;
  argv[argc++] = _gpgme_get_basename (pgmname);
  if (home_dir)
    {
      argv[argc++] = "--homedir";
      argv[argc++] = home_dir;
    }
  argv[argc++] = "--server";
  argv[argc++] = NULL;

  err = assuan_new_ext (&gpgsm->assuan_ctx, GPG_ERR_SOURCE_GPGME,
			&_gpgme_assuan_malloc_hooks, _gpgme_assuan_log_cb,
			NULL);
  if (err)
    goto leave;
  assuan_ctx_set_system_hooks (gpgsm->assuan_ctx, &_gpgme_assuan_system_hooks);

#if USE_DESCRIPTOR_PASSING
  err = assuan_pipe_connect (gpgsm->assuan_ctx, pgmname, argv,
                             NULL, NULL, NULL, ASSUAN_PIPE_CONNECT_FDPASSING);
#else
  {
    assuan_fd_t achild_fds[4];
    int i;

    /* For now... */
    for (i = 0; i < 4; i++)
      achild_fds[i] = (assuan_fd_t) child_fds[i];

    err = assuan_pipe_connect (gpgsm->assuan_ctx, pgmname, argv,
                               achild_fds, NULL, NULL, 0);

    /* For now... */
    for (i = 0; i < 4; i++)
      child_fds[i] = (int) achild_fds[i];
  }

  /* On Windows, handles are inserted in the spawned process with
     DuplicateHandle, and child_fds contains the server-local names
     for the inserted handles when assuan_pipe_connect returns.  */
  if (!err)
    {
      /* Note: We don't use _gpgme_io_fd2str here.  On W32 the
	 returned handles are real W32 system handles, not whatever
	 GPGME uses internally (which may be a system handle, a C
	 library handle or a GLib/Qt channel.  Confusing, yes, but
	 remember these are server-local names, so they are not part
	 of GPGME at all.  */
      snprintf (gpgsm->input_cb.server_fd_str,
		sizeof gpgsm->input_cb.server_fd_str, "%d", child_fds[0]);
      snprintf (gpgsm->output_cb.server_fd_str,
		sizeof gpgsm->output_cb.server_fd_str, "%d", child_fds[1]);
      snprintf (gpgsm->message_cb.server_fd_str,
		sizeof gpgsm->message_cb.server_fd_str, "%d", child_fds[2]);
    }
#endif
  if (err)
    goto leave;

  err = _gpgme_getenv ("DISPLAY", &dft_display);
  if (err)
    goto leave;
  if (dft_display)
    {
      if (asprintf (&optstr, "OPTION display=%s", dft_display) < 0)
        {
	  free (dft_display);
	  err = gpg_error_from_syserror ();
	  goto leave;
	}
      free (dft_display);

      err = assuan_transact (gpgsm->assuan_ctx, optstr, NULL, NULL, NULL,
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
	  err = assuan_transact (gpgsm->assuan_ctx, optstr, NULL, NULL, NULL,
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
		  free (dft_ttytype);
		  err = gpg_error_from_syserror ();
		  goto leave;
		}
	      free (dft_ttytype);

	      err = assuan_transact (gpgsm->assuan_ctx, optstr, NULL, NULL,
				     NULL, NULL, NULL, NULL);
	      free (optstr);
	      if (err)
		goto leave;
	    }
	}
    }

  /* Ask gpgsm to enable the audit log support.  */
  if (!err)
    {
      err = assuan_transact (gpgsm->assuan_ctx, "OPTION enable-audit-log=1",
                             NULL, NULL, NULL, NULL, NULL, NULL);
      if (gpg_err_code (err) == GPG_ERR_UNKNOWN_OPTION)
        err = 0; /* This is an optional feature of gpgsm.  */
    }


#ifdef HAVE_W32_SYSTEM
  /* Under Windows we need to use AllowSetForegroundWindow.  Tell
     gpgsm to tell us when it needs it.  */
  if (!err)
    {
      err = assuan_transact (gpgsm->assuan_ctx, "OPTION allow-pinentry-notify",
                             NULL, NULL, NULL, NULL, NULL, NULL);
      if (gpg_err_code (err) == GPG_ERR_UNKNOWN_OPTION)
        err = 0; /* This is a new feature of gpgsm.  */
    }
#endif /*HAVE_W32_SYSTEM*/

#if !USE_DESCRIPTOR_PASSING
  if (!err
      && (_gpgme_io_set_close_notify (gpgsm->input_cb.fd,
				      close_notify_handler, gpgsm)
	  || _gpgme_io_set_close_notify (gpgsm->output_cb.fd,
					 close_notify_handler, gpgsm)
	  || _gpgme_io_set_close_notify (gpgsm->message_cb.fd,
					 close_notify_handler, gpgsm)))
    {
      err = gpg_error (GPG_ERR_GENERAL);
      goto leave;
    }
#endif

 leave:
  /* Close the server ends of the pipes (because of this, we must use
     the stored server_fd_str in the function start).  Our ends are
     closed in gpgsm_release().  */
#if !USE_DESCRIPTOR_PASSING
  if (gpgsm->input_cb.server_fd != -1)
    _gpgme_io_close (gpgsm->input_cb.server_fd);
  if (gpgsm->output_cb.server_fd != -1)
    _gpgme_io_close (gpgsm->output_cb.server_fd);
  if (gpgsm->message_cb.server_fd != -1)
    _gpgme_io_close (gpgsm->message_cb.server_fd);
#endif

  if (err)
    gpgsm_release (gpgsm);
  else
    *engine = gpgsm;

  return err;
}
