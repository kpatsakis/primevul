_dbus_header_get_flag (DBusHeader   *header,
                       dbus_uint32_t flag)
{
  const unsigned char *flags_p;

  flags_p = _dbus_string_get_const_data_len (&header->data, FLAGS_OFFSET, 1);

  return (*flags_p & flag) != 0;
}
