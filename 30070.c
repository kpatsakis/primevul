_dbus_header_cache_invalidate_all (DBusHeader *header)
{
  int i;

  i = 0;
  while (i <= DBUS_HEADER_FIELD_LAST)
    {
      header->fields[i].value_pos = _DBUS_HEADER_FIELD_VALUE_UNKNOWN;
      ++i;
    }
}
