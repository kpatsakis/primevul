lock_encryption_data_new (DBusGMethodInvocation *context,
                          Device *luks_device,
                          Device *cleartext_device)
{
  LockEncryptionData *data;

  data = g_new0 (LockEncryptionData, 1);
  data->refcount = 1;

  data->context = context;
  data->luks_device = g_object_ref (luks_device);
  data->cleartext_device = g_object_ref (cleartext_device);
  return data;
}
