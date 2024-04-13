_dbus_header_set_field_basic (DBusHeader       *header,
                              int               field,
                              int               type,
                              const void       *value)
{
  _dbus_assert (field <= DBUS_HEADER_FIELD_LAST);

  if (!reserve_header_padding (header))
    return FALSE;

  /* If the field exists we set, otherwise we append */
  if (_dbus_header_cache_check (header, field))
    {
      DBusTypeReader reader;
      DBusTypeReader realign_root;

      if (!find_field_for_modification (header, field,
                                        &reader, &realign_root))
        _dbus_assert_not_reached ("field was marked present in cache but wasn't found");

      if (!set_basic_field (&reader, field, type, value, &realign_root))
        return FALSE;
    }
  else
    {
      DBusTypeWriter writer;
      DBusTypeWriter array;

      _dbus_type_writer_init_values_only (&writer,
                                          header->byte_order,
                                          &_dbus_header_signature_str,
                                          FIELDS_ARRAY_SIGNATURE_OFFSET,
                                          &header->data,
                                          FIELDS_ARRAY_LENGTH_OFFSET);

      /* recurse into array without creating a new length, and jump to
       * end of array.
       */
      if (!_dbus_type_writer_append_array (&writer,
                                           &_dbus_header_signature_str,
                                           FIELDS_ARRAY_ELEMENT_SIGNATURE_OFFSET,
                                           &array))
        _dbus_assert_not_reached ("recurse into ARRAY should not have used memory");

      _dbus_assert (array.u.array.len_pos == FIELDS_ARRAY_LENGTH_OFFSET);
      _dbus_assert (array.u.array.start_pos == FIRST_FIELD_OFFSET);
      _dbus_assert (array.value_pos == HEADER_END_BEFORE_PADDING (header));

      if (!write_basic_field (&array,
                              field, type, value))
        return FALSE;

      if (!_dbus_type_writer_unrecurse (&writer, &array))
        _dbus_assert_not_reached ("unrecurse from ARRAY should not have used memory");
    }

  correct_header_padding (header);

  /* We could be smarter about this (only invalidate fields after the
   * one we modified, or even only if the one we modified changed
   * length). But this hack is a start.
   */
  _dbus_header_cache_invalidate_all (header);

  return TRUE;
}
