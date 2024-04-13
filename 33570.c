gpgsm_assuan_simple_command (assuan_context_t ctx, char *cmd,
			     engine_status_handler_t status_fnc,
			     void *status_fnc_value)
{
  gpg_error_t err;
  char *line;
  size_t linelen;

  err = assuan_write_line (ctx, cmd);
  if (err)
    return err;

  do
    {
      err = assuan_read_line (ctx, &line, &linelen);
      if (err)
	return err;

      if (*line == '#' || !linelen)
	continue;

      if (linelen >= 2
	  && line[0] == 'O' && line[1] == 'K'
	  && (line[2] == '\0' || line[2] == ' '))
	return 0;
      else if (linelen >= 4
	  && line[0] == 'E' && line[1] == 'R' && line[2] == 'R'
	  && line[3] == ' ')
	err = atoi (&line[4]);
      else if (linelen >= 2
	       && line[0] == 'S' && line[1] == ' ')
	{
	  char *rest;
	  gpgme_status_code_t r;

	  rest = strchr (line + 2, ' ');
	  if (!rest)
	    rest = line + linelen; /* set to an empty string */
	  else
	    *(rest++) = 0;

	  r = _gpgme_parse_status (line + 2);

	  if (r >= 0 && status_fnc)
	    err = status_fnc (status_fnc_value, r, rest);
	  else
	    err = gpg_error (GPG_ERR_GENERAL);
	}
      else
	err = gpg_error (GPG_ERR_GENERAL);
    }
  while (!err);

  return err;
}
