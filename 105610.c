unlock_encryption_data_ref (UnlockEncryptionData *data)
{
  data->refcount++;
  return data;
}
