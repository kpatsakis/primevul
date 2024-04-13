_dbus_header_cache_revalidate (DBusHeader *header)
{
  DBusTypeReader array;
  DBusTypeReader reader;
  int i;

  i = 0;
  while (i <= DBUS_HEADER_FIELD_LAST)
    {
      header->fields[i].value_pos = _DBUS_HEADER_FIELD_VALUE_NONEXISTENT;
      ++i;
    }

  _dbus_type_reader_init (&reader,
                          header->byte_order,
                          &_dbus_header_signature_str,
                          FIELDS_ARRAY_SIGNATURE_OFFSET,
                          &header->data,
                          FIELDS_ARRAY_LENGTH_OFFSET);

  _dbus_type_reader_recurse (&reader, &array);

  while (_dbus_type_reader_get_current_type (&array) != DBUS_TYPE_INVALID)
    {
      DBusTypeReader sub;
      DBusTypeReader variant;
      unsigned char field_code;

      _dbus_type_reader_recurse (&array, &sub);

      _dbus_assert (_dbus_type_reader_get_current_type (&sub) == DBUS_TYPE_BYTE);
      _dbus_type_reader_read_basic (&sub, &field_code);

      /* Unknown fields should be ignored */
      if (field_code > DBUS_HEADER_FIELD_LAST)
        goto next_field;

      _dbus_type_reader_next (&sub);

      _dbus_assert (_dbus_type_reader_get_current_type (&sub) == DBUS_TYPE_VARIANT);
      _dbus_type_reader_recurse (&sub, &variant);

      _dbus_header_cache_one (header, field_code, &variant);

    next_field:
      _dbus_type_reader_next (&array);
    }
}
