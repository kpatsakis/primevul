  ps_parser_load_field_table( PS_Parser       parser,
                              const T1_Field  field,
                              void**          objects,
                              FT_UInt         max_objects,
                              FT_ULong*       pflags )
  {
    T1_TokenRec  elements[T1_MAX_TABLE_ELEMENTS];
    T1_Token     token;
    FT_Int       num_elements;
    FT_Error     error = FT_Err_Ok;
    FT_Byte*     old_cursor;
    FT_Byte*     old_limit;
    T1_FieldRec  fieldrec = *(T1_Field)field;


    fieldrec.type = T1_FIELD_TYPE_INTEGER;
    if ( field->type == T1_FIELD_TYPE_FIXED_ARRAY ||
         field->type == T1_FIELD_TYPE_BBOX        )
      fieldrec.type = T1_FIELD_TYPE_FIXED;

    ps_parser_to_token_array( parser, elements,
                              T1_MAX_TABLE_ELEMENTS, &num_elements );
    if ( num_elements < 0 )
    {
      error = FT_ERR( Ignore );
      goto Exit;
    }
    if ( (FT_UInt)num_elements > field->array_max )
      num_elements = (FT_Int)field->array_max;

    old_cursor = parser->cursor;
    old_limit  = parser->limit;

    /* we store the elements count if necessary;           */
    /* we further assume that `count_offset' can't be zero */
    if ( field->type != T1_FIELD_TYPE_BBOX && field->count_offset != 0 )
      *(FT_Byte*)( (FT_Byte*)objects[0] + field->count_offset ) =
        (FT_Byte)num_elements;

    /* we now load each element, adjusting the field.offset on each one */
    token = elements;
    for ( ; num_elements > 0; num_elements--, token++ )
    {
      parser->cursor = token->start;
      parser->limit  = token->limit;

      error = ps_parser_load_field( parser,
                                    &fieldrec,
                                    objects,
                                    max_objects,
                                    0 );
      if ( error )
        break;

      fieldrec.offset += fieldrec.size;
    }

#if 0  /* obsolete -- keep for reference */
    if ( pflags )
      *pflags |= 1L << field->flag_bit;
#else
    FT_UNUSED( pflags );
#endif

    parser->cursor = old_cursor;
    parser->limit  = old_limit;

  Exit:
    return error;
  }
