_dbus_header_create (DBusHeader  *header,
                     int          message_type,
                     const char  *destination,
                     const char  *path,
                     const char  *interface,
                     const char  *member,
                     const char  *error_name)
{
  unsigned char v_BYTE;
  dbus_uint32_t v_UINT32;
  DBusTypeWriter writer;
  DBusTypeWriter array;

  _dbus_assert (((interface || message_type != DBUS_MESSAGE_TYPE_SIGNAL) && member) ||
                (error_name) ||
                !(interface || member || error_name));
  _dbus_assert (_dbus_string_get_length (&header->data) == 0);

  if (!reserve_header_padding (header))
    return FALSE;

  _dbus_type_writer_init_values_only (&writer, header->byte_order,
                                      &_dbus_header_signature_str, 0,
                                      &header->data,
                                      HEADER_END_BEFORE_PADDING (header));

  v_BYTE = header->byte_order;
  if (!_dbus_type_writer_write_basic (&writer, DBUS_TYPE_BYTE,
                                      &v_BYTE))
    goto oom;

  v_BYTE = message_type;
  if (!_dbus_type_writer_write_basic (&writer, DBUS_TYPE_BYTE,
                                      &v_BYTE))
    goto oom;

  v_BYTE = 0; /* flags */
  if (!_dbus_type_writer_write_basic (&writer, DBUS_TYPE_BYTE,
                                      &v_BYTE))
    goto oom;

  v_BYTE = DBUS_MAJOR_PROTOCOL_VERSION;
  if (!_dbus_type_writer_write_basic (&writer, DBUS_TYPE_BYTE,
                                      &v_BYTE))
    goto oom;

  v_UINT32 = 0; /* body length */
  if (!_dbus_type_writer_write_basic (&writer, DBUS_TYPE_UINT32,
                                      &v_UINT32))
    goto oom;

  v_UINT32 = 0; /* serial */
  if (!_dbus_type_writer_write_basic (&writer, DBUS_TYPE_UINT32,
                                      &v_UINT32))
    goto oom;

  if (!_dbus_type_writer_recurse (&writer, DBUS_TYPE_ARRAY,
                                  &_dbus_header_signature_str,
                                  FIELDS_ARRAY_SIGNATURE_OFFSET,
                                  &array))
    goto oom;

  /* Marshal all the fields (Marshall Fields?) */

  if (path != NULL)
    {
      if (!write_basic_field (&array,
                              DBUS_HEADER_FIELD_PATH,
                              DBUS_TYPE_OBJECT_PATH,
                              &path))
        goto oom;
    }

  if (destination != NULL)
    {
      if (!write_basic_field (&array,
                              DBUS_HEADER_FIELD_DESTINATION,
                              DBUS_TYPE_STRING,
                              &destination))
        goto oom;
    }

  if (interface != NULL)
    {
      if (!write_basic_field (&array,
                              DBUS_HEADER_FIELD_INTERFACE,
                              DBUS_TYPE_STRING,
                              &interface))
        goto oom;
    }

  if (member != NULL)
    {
      if (!write_basic_field (&array,
                              DBUS_HEADER_FIELD_MEMBER,
                              DBUS_TYPE_STRING,
                              &member))
        goto oom;
    }

  if (error_name != NULL)
    {
      if (!write_basic_field (&array,
                              DBUS_HEADER_FIELD_ERROR_NAME,
                              DBUS_TYPE_STRING,
                              &error_name))
        goto oom;
    }

  if (!_dbus_type_writer_unrecurse (&writer, &array))
    goto oom;

  correct_header_padding (header);

  return TRUE;

 oom:
  _dbus_string_delete (&header->data, 0,
                       _dbus_string_get_length (&header->data) - header->padding);
  correct_header_padding (header);

  return FALSE;
}
