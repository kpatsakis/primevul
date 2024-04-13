luks_unlock_completed_cb (DBusGMethodInvocation *context,
                          Device *device,
                          gboolean job_was_cancelled,
                          int status,
                          const char *stderr,
                          const char *stdout,
                          gpointer user_data)
{
  UnlockEncryptionData *data = user_data;

  if (WEXITSTATUS (status) == 0 && !job_was_cancelled)
    {

      luks_unlock_start_waiting_for_cleartext_device (unlock_encryption_data_ref (data));

    }
  else
    {
      if (job_was_cancelled)
        {
          throw_error (context, ERROR_CANCELLED, "Job was cancelled");
        }
      else
        {
          throw_error (context,
                       ERROR_FAILED,
                       "Error unlocking device: cryptsetup exited with exit code %d: %s",
                       WEXITSTATUS (status),
                       stderr);
        }
      if (data->hook_func != NULL)
        {
          data->hook_func (data->context, NULL, data->hook_user_data);
        }
    }
}
