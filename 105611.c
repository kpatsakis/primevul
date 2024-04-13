unlock_encryption_data_unref (UnlockEncryptionData *data)
{
  data->refcount--;
  if (data->refcount == 0)
    {
      g_object_unref (data->device);
      g_free (data);
    }
}
