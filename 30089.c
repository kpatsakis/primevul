find_field_for_modification (DBusHeader     *header,
                             int             field,
                             DBusTypeReader *reader,
                             DBusTypeReader *realign_root)
{
  dbus_bool_t retval;

  retval = FALSE;

  _dbus_type_reader_init (realign_root,
                          header->byte_order,
                          &_dbus_header_signature_str,
                          FIELDS_ARRAY_SIGNATURE_OFFSET,
                          &header->data,
                          FIELDS_ARRAY_LENGTH_OFFSET);

  _dbus_type_reader_recurse (realign_root, reader);

  while (_dbus_type_reader_get_current_type (reader) != DBUS_TYPE_INVALID)
    {
      DBusTypeReader sub;
      unsigned char field_code;

      _dbus_type_reader_recurse (reader, &sub);

      _dbus_assert (_dbus_type_reader_get_current_type (&sub) == DBUS_TYPE_BYTE);
      _dbus_type_reader_read_basic (&sub, &field_code);

      if (field_code == (unsigned) field)
        {
          _dbus_assert (_dbus_type_reader_get_current_type (reader) == DBUS_TYPE_STRUCT);
          retval = TRUE;
          goto done;
        }

      _dbus_type_reader_next (reader);
    }

 done:
  return retval;
}
