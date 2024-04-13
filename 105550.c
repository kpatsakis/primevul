lock_encryption_data_unref (LockEncryptionData *data)
{
  data->refcount--;
  if (data->refcount == 0)
    {
      g_object_unref (data->luks_device);
      g_object_unref (data->cleartext_device);
      g_free (data);
    }
}
