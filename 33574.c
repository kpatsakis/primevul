gpgsm_delete (void *engine, gpgme_key_t key, int allow_secret)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;
  char *fpr = key->subkeys ? key->subkeys->fpr : NULL;
  char *linep = fpr;
  char *line;
  int length = 8;	/* "DELKEYS " */

  if (!fpr)
    return gpg_error (GPG_ERR_INV_VALUE);

  while (*linep)
    {
      length++;
      if (*linep == '%' || *linep == ' ' || *linep == '+')
	length += 2;
      linep++;
    }
  length++;

  line = malloc (length);
  if (!line)
    return gpg_error_from_syserror ();

  strcpy (line, "DELKEYS ");
  linep = &line[8];

  while (*fpr)
    {
      switch (*fpr)
	{
	case '%':
	  *(linep++) = '%';
	  *(linep++) = '2';
	  *(linep++) = '5';
	  break;
	case ' ':
	  *(linep++) = '%';
	  *(linep++) = '2';
	  *(linep++) = '0';
	  break;
	case '+':
	  *(linep++) = '%';
	  *(linep++) = '2';
	  *(linep++) = 'B';
	  break;
	default:
	  *(linep++) = *fpr;
	  break;
	}
      fpr++;
    }
  *linep = '\0';

  gpgsm_clear_fd (gpgsm, OUTPUT_FD);
  gpgsm_clear_fd (gpgsm, INPUT_FD);
  gpgsm_clear_fd (gpgsm, MESSAGE_FD);
  gpgsm->inline_data = NULL;

  err = start (gpgsm, line);
  free (line);

  return err;
}
