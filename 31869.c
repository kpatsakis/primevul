iter_first_in_series (DBusMessageDataIter *iter)
{
  int i;

  i = iter->depth;
  while (i < _DBUS_MESSAGE_DATA_MAX_NESTING)
    {
      if (iter->sequence_nos[i] != 0)
        return FALSE;
      ++i;
    }
  return TRUE;
}
