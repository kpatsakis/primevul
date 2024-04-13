gpgsm_import (void *engine, gpgme_data_t keydata, gpgme_key_t *keyarray)
{
  engine_gpgsm_t gpgsm = engine;
  gpgme_error_t err;
  gpgme_data_encoding_t dataenc;
  int idx;

  if (!gpgsm)
    return gpg_error (GPG_ERR_INV_VALUE);

  if (keydata && keyarray)
    return gpg_error (GPG_ERR_INV_VALUE); /* Only one is allowed.  */

  dataenc = gpgme_data_get_encoding (keydata);

  if (keyarray)
    {
      size_t buflen;
      char *buffer, *p;

      /* Fist check whether the engine already features the
         --re-import option.  */
      err = gpgsm_assuan_simple_command
        (gpgsm->assuan_ctx,
         "GETINFO cmd_has_option IMPORT re-import", NULL, NULL);
      if (err)
	return gpg_error (GPG_ERR_NOT_SUPPORTED);

      /* Create an internal data object with a list of all
         fingerprints.  The data object and its memory (to avoid an
         extra copy by gpgme_data_new_from_mem) are stored in two
         variables which are released by the close_notify_handler.  */
      for (idx=0, buflen=0; keyarray[idx]; idx++)
        {
          if (keyarray[idx]->protocol == GPGME_PROTOCOL_CMS
              && keyarray[idx]->subkeys
              && keyarray[idx]->subkeys->fpr
              && *keyarray[idx]->subkeys->fpr)
            buflen += strlen (keyarray[idx]->subkeys->fpr) + 1;
        }
      /* Allocate a bufer with extra space for the trailing Nul
         introduced by the use of stpcpy.  */
      buffer = malloc (buflen+1);
      if (!buffer)
        return gpg_error_from_syserror ();
      for (idx=0, p = buffer; keyarray[idx]; idx++)
        {
          if (keyarray[idx]->protocol == GPGME_PROTOCOL_CMS
              && keyarray[idx]->subkeys
              && keyarray[idx]->subkeys->fpr
              && *keyarray[idx]->subkeys->fpr)
            p = stpcpy (stpcpy (p, keyarray[idx]->subkeys->fpr), "\n");
        }

      err = gpgme_data_new_from_mem (&gpgsm->input_helper_data,
                                     buffer, buflen, 0);
      if (err)
        {
          free (buffer);
          return err;
        }
      gpgsm->input_helper_memory = buffer;

      gpgsm->input_cb.data = gpgsm->input_helper_data;
      err = gpgsm_set_fd (gpgsm, INPUT_FD, map_data_enc (gpgsm->input_cb.data));
      if (err)
        {
          gpgme_data_release (gpgsm->input_helper_data);
          gpgsm->input_helper_data = NULL;
          free (gpgsm->input_helper_memory);
          gpgsm->input_helper_memory = NULL;
          return err;
        }
      gpgsm_clear_fd (gpgsm, OUTPUT_FD);
      gpgsm_clear_fd (gpgsm, MESSAGE_FD);
      gpgsm->inline_data = NULL;

      return start (gpgsm, "IMPORT --re-import");
    }
  else if (dataenc == GPGME_DATA_ENCODING_URL
           || dataenc == GPGME_DATA_ENCODING_URL0
           || dataenc == GPGME_DATA_ENCODING_URLESC)
    {
      return gpg_error (GPG_ERR_NOT_IMPLEMENTED);
    }
  else
    {
      gpgsm->input_cb.data = keydata;
      err = gpgsm_set_fd (gpgsm, INPUT_FD, map_data_enc (gpgsm->input_cb.data));
      if (err)
        return err;
      gpgsm_clear_fd (gpgsm, OUTPUT_FD);
      gpgsm_clear_fd (gpgsm, MESSAGE_FD);
      gpgsm->inline_data = NULL;

      return start (gpgsm, "IMPORT");
    }
}
