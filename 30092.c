write_basic_field (DBusTypeWriter *writer,
                   int             field,
                   int             type,
                   const void     *value)
{
  DBusTypeWriter sub;
  DBusTypeWriter variant;
  int start;
  int padding;
  unsigned char field_byte;
  DBusString contained_type;
  char buf[2];

  start = writer->value_pos;
  padding = _dbus_string_get_length (writer->value_str) - start;

  if (!_dbus_type_writer_recurse (writer, DBUS_TYPE_STRUCT,
                                  NULL, 0, &sub))
    goto append_failed;

  field_byte = field;
  if (!_dbus_type_writer_write_basic (&sub, DBUS_TYPE_BYTE,
                                      &field_byte))
    goto append_failed;

  buf[0] = type;
  buf[1] = '\0';
  _dbus_string_init_const_len (&contained_type, buf, 1);

  if (!_dbus_type_writer_recurse (&sub, DBUS_TYPE_VARIANT,
                                  &contained_type, 0, &variant))
    goto append_failed;

  if (!_dbus_type_writer_write_basic (&variant, type, value))
    goto append_failed;

  if (!_dbus_type_writer_unrecurse (&sub, &variant))
    goto append_failed;

  if (!_dbus_type_writer_unrecurse (writer, &sub))
    goto append_failed;

  return TRUE;

 append_failed:
  _dbus_string_delete (writer->value_str,
                       start,
                       _dbus_string_get_length (writer->value_str) - start - padding);
  return FALSE;
}
