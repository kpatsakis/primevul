reserve_header_padding (DBusHeader *header)
{
  _dbus_assert (header->padding <= MAX_POSSIBLE_HEADER_PADDING);

  if (!_dbus_string_lengthen (&header->data,
                              MAX_POSSIBLE_HEADER_PADDING - header->padding))
    return FALSE;
  header->padding = MAX_POSSIBLE_HEADER_PADDING;
  return TRUE;
}
