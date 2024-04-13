mkfse_data_unref (MkfsLuksData *data)
{
  data->refcount--;
  if (data->refcount == 0)
    {
      if (data->passphrase != NULL)
        {
          memset (data->passphrase, '\0', strlen (data->passphrase));
          g_free (data->passphrase);
        }
      if (data->device != NULL)
        g_object_unref (data->device);
      g_strfreev (data->options);
      g_free (data->fstype);
      g_free (data);
    }
}
