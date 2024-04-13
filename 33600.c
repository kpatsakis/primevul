set_recipients (engine_uiserver_t uiserver, gpgme_key_t recp[])
{
  gpgme_error_t err = 0;
  assuan_context_t ctx = uiserver->assuan_ctx;
  char *line;
  int linelen;
  int invalid_recipients = 0;
  int i;

  linelen = 10 + 40 + 1;	/* "RECIPIENT " + guess + '\0'.  */
  line = malloc (10 + 40 + 1);
  if (!line)
    return gpg_error_from_syserror ();
  strcpy (line, "RECIPIENT ");
  for (i=0; !err && recp[i]; i++)
    {
      char *uid;
      int newlen;

      /* We use only the first user ID of the key.  */
      if (!recp[i]->uids || !(uid=recp[i]->uids->uid) || !*uid)
	{
	  invalid_recipients++;
	  continue;
	}

      newlen = 11 + strlen (uid);
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
      /* FIXME: need to do proper escaping  */
      strcpy (&line[10], uid);

      err = uiserver_assuan_simple_command (ctx, line, uiserver->status.fnc,
                                            uiserver->status.fnc_value);
      /* FIXME: This might requires more work.  */
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
