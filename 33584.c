gpgsm_keylist (void *engine, const char *pattern, int secret_only,
	       gpgme_keylist_mode_t mode)
{
  engine_gpgsm_t gpgsm = engine;
  char *line;
  gpgme_error_t err;
  int list_mode = 0;

  if (mode & GPGME_KEYLIST_MODE_LOCAL)
    list_mode |= 1;
  if (mode & GPGME_KEYLIST_MODE_EXTERN)
    list_mode |= 2;

  if (!pattern)
    pattern = "";

  /* Hack to make sure that the agent is started.  Only if the agent
     has been started an application may connect to the agent via
     GPGME_PROTOCOL_ASSUAN - for example to look for smartcards.  We
     do this only if a secret key listing has been requested.  In
     general this is not needed because a secret key listing starts
     the agent.  However on a fresh installation no public keys are
     available and thus there is no need for gpgsm to ask the agent
     whether a secret key exists for the public key.  */
  if (secret_only || (mode & GPGME_KEYLIST_MODE_WITH_SECRET))
    gpgsm_assuan_simple_command (gpgsm->assuan_ctx, "GETINFO agent-check",
                                 NULL, NULL);

  /* Always send list-mode option because RESET does not reset it.  */
  if (asprintf (&line, "OPTION list-mode=%d", (list_mode & 3)) < 0)
    return gpg_error_from_syserror ();
  err = gpgsm_assuan_simple_command (gpgsm->assuan_ctx, line, NULL, NULL);
  free (line);
  if (err)
    return err;


  /* Always send key validation because RESET does not reset it.  */

  /* Use the validation mode if requested.  We don't check for an error
     yet because this is a pretty fresh gpgsm features. */
  gpgsm_assuan_simple_command (gpgsm->assuan_ctx,
                               (mode & GPGME_KEYLIST_MODE_VALIDATE)?
                               "OPTION with-validation=1":
                               "OPTION with-validation=0" ,
                               NULL, NULL);
  /* Include the ephemeral keys if requested.  We don't check for an error
     yet because this is a pretty fresh gpgsm features. */
  gpgsm_assuan_simple_command (gpgsm->assuan_ctx,
                               (mode & GPGME_KEYLIST_MODE_EPHEMERAL)?
                               "OPTION with-ephemeral-keys=1":
                               "OPTION with-ephemeral-keys=0" ,
                               NULL, NULL);
  gpgsm_assuan_simple_command (gpgsm->assuan_ctx,
                               (mode & GPGME_KEYLIST_MODE_WITH_SECRET)?
                               "OPTION with-secret=1":
                               "OPTION with-secret=0" ,
                               NULL, NULL);


  /* Length is "LISTSECRETKEYS " + p + '\0'.  */
  line = malloc (15 + strlen (pattern) + 1);
  if (!line)
    return gpg_error_from_syserror ();
  if (secret_only)
    {
      strcpy (line, "LISTSECRETKEYS ");
      strcpy (&line[15], pattern);
    }
  else
    {
      strcpy (line, "LISTKEYS ");
      strcpy (&line[9], pattern);
    }

  gpgsm_clear_fd (gpgsm, INPUT_FD);
  gpgsm_clear_fd (gpgsm, OUTPUT_FD);
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, line);
  free (line);
  return err;
}
