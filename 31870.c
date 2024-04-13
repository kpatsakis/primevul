iter_get_sequence (DBusMessageDataIter *iter)
{
  _dbus_assert (iter->sequence_nos[iter->depth] >= 0);
  return iter->sequence_nos[iter->depth];
}
