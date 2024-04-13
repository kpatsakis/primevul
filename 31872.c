iter_unrecurse (DBusMessageDataIter *iter)
{
  iter->depth -= 1;
  _dbus_assert (iter->depth >= 0);
}
