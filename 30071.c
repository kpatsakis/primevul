_dbus_header_cache_known_nonexistent (DBusHeader    *header,
                                      int            field)
{
  _dbus_assert (field <= DBUS_HEADER_FIELD_LAST);

  return (header->fields[field].value_pos == _DBUS_HEADER_FIELD_VALUE_NONEXISTENT);
}
