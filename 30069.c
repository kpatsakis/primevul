_dbus_header_cache_check (DBusHeader    *header,
                          int            field)
{
  _dbus_assert (field <= DBUS_HEADER_FIELD_LAST);

  if (header->fields[field].value_pos == _DBUS_HEADER_FIELD_VALUE_UNKNOWN)
    _dbus_header_cache_revalidate (header);

  if (header->fields[field].value_pos == _DBUS_HEADER_FIELD_VALUE_NONEXISTENT)
    return FALSE;

  return TRUE;
}
