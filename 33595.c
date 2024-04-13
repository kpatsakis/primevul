set_recipients (engine_gpgsm_t gpgsm, gpgme_key_t recp[])
{
  gpgme_error_t err = 0;
  assuan_context_t ctx = gpgsm->assuan_ctx;
  char *line;
  int linelen;
  int invalid_recipients = 0;
  int i;

  linelen = 10 + 40 + 1;	/* "RECIPIENT " + guess + '\0'.  */
  line = malloc (10 + 40 + 1);
  if (!line)
    return gpg_error_from_syserror ();
  strcpy (line, "RECIPIENT ");
  for (i =0; !err && recp[i]; i++)
    {
      char *fpr;
      int newlen;

      if (!recp[i]->subkeys || !recp[i]->subkeys->fpr)
	{
	  invalid_recipients++;
	  continue;
	}
      fpr = recp[i]->subkeys->fpr;

      newlen = 11 + strlen (fpr);
      if (linelen < newlen)
	{
	  char *newline = realloc (line, newlen);
	  if (! newline)
	    {
	      int saved_err = gpg_error_from_syserror ();
	      free (line);
	      return saved_err;
	    }
	  line = newline;
	  linelen = newlen;
	}
      strcpy (&line[10], fpr);

      err = gpgsm_assuan_simple_command (ctx, line, gpgsm->status.fnc,
					 gpgsm->status.fnc_value);
      /* FIXME: This requires more work.  */
      if (gpg_err_code (err) == GPG_ERR_NO_PUBKEY)
	invalid_recipients++;
      else if (err)
	{
	  free (line);
	  return err;
	}
    }
  free (line);
  return gpg_error (invalid_recipients
		    ? GPG_ERR_UNUSABLE_PUBKEY : GPG_ERR_NO_ERROR);
}
