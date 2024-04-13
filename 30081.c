_dbus_header_have_message_untrusted (int                max_message_length,
                                     DBusValidity      *validity,
                                     int               *byte_order,
                                     int               *fields_array_len,
                                     int               *header_len,
                                     int               *body_len,
                                     const DBusString  *str,
                                     int                start,
                                     int                len)

{
  dbus_uint32_t header_len_unsigned;
  dbus_uint32_t fields_array_len_unsigned;
  dbus_uint32_t body_len_unsigned;

  _dbus_assert (start >= 0);
  _dbus_assert (start < _DBUS_INT32_MAX / 2);
  _dbus_assert (len >= 0);

  _dbus_assert (start == (int) _DBUS_ALIGN_VALUE (start, 8));

  *byte_order = _dbus_string_get_byte (str, start + BYTE_ORDER_OFFSET);

  if (*byte_order != DBUS_LITTLE_ENDIAN && *byte_order != DBUS_BIG_ENDIAN)
    {
      *validity = DBUS_INVALID_BAD_BYTE_ORDER;
      return FALSE;
    }

  _dbus_assert (FIELDS_ARRAY_LENGTH_OFFSET + 4 <= len);
  fields_array_len_unsigned = _dbus_marshal_read_uint32 (str, start + FIELDS_ARRAY_LENGTH_OFFSET,
                                                         *byte_order, NULL);

  if (fields_array_len_unsigned > (unsigned) max_message_length)
    {
      *validity = DBUS_INVALID_INSANE_FIELDS_ARRAY_LENGTH;
      return FALSE;
    }

  _dbus_assert (BODY_LENGTH_OFFSET + 4 < len);
  body_len_unsigned = _dbus_marshal_read_uint32 (str, start + BODY_LENGTH_OFFSET,
                                                 *byte_order, NULL);

  if (body_len_unsigned > (unsigned) max_message_length)
    {
      *validity = DBUS_INVALID_INSANE_BODY_LENGTH;
      return FALSE;
    }

  header_len_unsigned = FIRST_FIELD_OFFSET + fields_array_len_unsigned;
  header_len_unsigned = _DBUS_ALIGN_VALUE (header_len_unsigned, 8);

  /* overflow should be impossible since the lengths aren't allowed to
   * be huge.
   */
  _dbus_assert (max_message_length < _DBUS_INT32_MAX / 2);
  if (body_len_unsigned + header_len_unsigned > (unsigned) max_message_length)
    {
      *validity = DBUS_INVALID_MESSAGE_TOO_LONG;
      return FALSE;
    }

  _dbus_assert (body_len_unsigned < (unsigned) _DBUS_INT32_MAX);
  _dbus_assert (fields_array_len_unsigned < (unsigned) _DBUS_INT32_MAX);
  _dbus_assert (header_len_unsigned < (unsigned) _DBUS_INT32_MAX);

  *body_len = body_len_unsigned;
  *fields_array_len = fields_array_len_unsigned;
  *header_len = header_len_unsigned;

  *validity = DBUS_VALID;

  _dbus_verbose ("have %d bytes, need body %u + header %u = %u\n",
                 len, body_len_unsigned, header_len_unsigned,
                 body_len_unsigned + header_len_unsigned);

  return (body_len_unsigned + header_len_unsigned) <= (unsigned) len;
}
