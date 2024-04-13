lock_encryption_data_ref (LockEncryptionData *data)
{
  data->refcount++;
  return data;
}
