_dbus_header_toggle_flag (DBusHeader   *header,
                          dbus_uint32_t flag,
                          dbus_bool_t   value)
{
  unsigned char *flags_p;

  flags_p = _dbus_string_get_data_len (&header->data, FLAGS_OFFSET, 1);

  if (value)
    *flags_p |= flag;
  else
    *flags_p &= ~flag;
}
