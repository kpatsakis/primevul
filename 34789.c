  ps_parser_load_field( PS_Parser       parser,
                        const T1_Field  field,
                        void**          objects,
                        FT_UInt         max_objects,
                        FT_ULong*       pflags )
  {
    T1_TokenRec   token;
    FT_Byte*      cur;
    FT_Byte*      limit;
    FT_UInt       count;
    FT_UInt       idx;
    FT_Error      error;
    T1_FieldType  type;


    /* this also skips leading whitespace */
    ps_parser_to_token( parser, &token );
    if ( !token.type )
      goto Fail;

    count = 1;
    idx   = 0;
    cur   = token.start;
    limit = token.limit;

    type = field->type;

    /* we must detect arrays in /FontBBox */
    if ( type == T1_FIELD_TYPE_BBOX )
    {
      T1_TokenRec  token2;
      FT_Byte*     old_cur   = parser->cursor;
      FT_Byte*     old_limit = parser->limit;


      /* don't include delimiters */
      parser->cursor = token.start + 1;
      parser->limit  = token.limit - 1;

      ps_parser_to_token( parser, &token2 );
      parser->cursor = old_cur;
      parser->limit  = old_limit;

      if ( token2.type == T1_TOKEN_TYPE_ARRAY )
      {
        type = T1_FIELD_TYPE_MM_BBOX;
        goto FieldArray;
      }
    }
    else if ( token.type == T1_TOKEN_TYPE_ARRAY )
    {
      count = max_objects;

    FieldArray:
      /* if this is an array and we have no blend, an error occurs */
      if ( max_objects == 0 )
        goto Fail;

      idx = 1;

      /* don't include delimiters */
      cur++;
      limit--;
    }

    for ( ; count > 0; count--, idx++ )
    {
      FT_Byte*    q = (FT_Byte*)objects[idx] + field->offset;
      FT_Long     val;
      FT_String*  string;


      skip_spaces( &cur, limit );

      switch ( type )
      {
      case T1_FIELD_TYPE_BOOL:
        val = ps_tobool( &cur, limit );
        goto Store_Integer;

      case T1_FIELD_TYPE_FIXED:
        val = PS_Conv_ToFixed( &cur, limit, 0 );
        goto Store_Integer;

      case T1_FIELD_TYPE_FIXED_1000:
        val = PS_Conv_ToFixed( &cur, limit, 3 );
        goto Store_Integer;

      case T1_FIELD_TYPE_INTEGER:
        val = PS_Conv_ToInt( &cur, limit );
        /* fall through */

      Store_Integer:
        switch ( field->size )
        {
        case (8 / FT_CHAR_BIT):
          *(FT_Byte*)q = (FT_Byte)val;
          break;

        case (16 / FT_CHAR_BIT):
          *(FT_UShort*)q = (FT_UShort)val;
          break;

        case (32 / FT_CHAR_BIT):
          *(FT_UInt32*)q = (FT_UInt32)val;
          break;

        default:                /* for 64-bit systems */
          *(FT_Long*)q = val;
        }
        break;

      case T1_FIELD_TYPE_STRING:
      case T1_FIELD_TYPE_KEY:
        {
          FT_Memory  memory = parser->memory;
          FT_UInt    len    = (FT_UInt)( limit - cur );


          if ( cur >= limit )
            break;

          /* we allow both a string or a name   */
          /* for cases like /FontName (foo) def */
          if ( token.type == T1_TOKEN_TYPE_KEY )
          {
            /* don't include leading `/' */
            len--;
            cur++;
          }
          else if ( token.type == T1_TOKEN_TYPE_STRING )
          {
            /* don't include delimiting parentheses    */
            /* XXX we don't handle <<...>> here        */
            /* XXX should we convert octal escapes?    */
            /*     if so, what encoding should we use? */
            cur++;
            len -= 2;
          }
          else
          {
            FT_ERROR(( "ps_parser_load_field:"
                       " expected a name or string\n"
                       "                     "
                       " but found token of type %d instead\n",
                       token.type ));
            error = FT_THROW( Invalid_File_Format );
            goto Exit;
          }

          /* for this to work (FT_String**)q must have been */
          /* initialized to NULL                            */
          if ( *(FT_String**)q != NULL )
          {
            FT_TRACE0(( "ps_parser_load_field: overwriting field %s\n",
                        field->ident ));
            FT_FREE( *(FT_String**)q );
            *(FT_String**)q = NULL;
          }

          if ( FT_ALLOC( string, len + 1 ) )
            goto Exit;

          FT_MEM_COPY( string, cur, len );
          string[len] = 0;

          *(FT_String**)q = string;
        }
        break;

      case T1_FIELD_TYPE_BBOX:
        {
          FT_Fixed  temp[4];
          FT_BBox*  bbox = (FT_BBox*)q;
          FT_Int    result;


          result = ps_tofixedarray( &cur, limit, 4, temp, 0 );

          if ( result < 4 )
          {
            FT_ERROR(( "ps_parser_load_field:"
                       " expected four integers in bounding box\n" ));
            error = FT_THROW( Invalid_File_Format );
            goto Exit;
          }

          bbox->xMin = FT_RoundFix( temp[0] );
          bbox->yMin = FT_RoundFix( temp[1] );
          bbox->xMax = FT_RoundFix( temp[2] );
          bbox->yMax = FT_RoundFix( temp[3] );
        }
        break;

      case T1_FIELD_TYPE_MM_BBOX:
        {
          FT_Memory  memory = parser->memory;
          FT_Fixed*  temp;
          FT_Int     result;
          FT_UInt    i;


          if ( FT_NEW_ARRAY( temp, max_objects * 4 ) )
            goto Exit;

          for ( i = 0; i < 4; i++ )
          {
            result = ps_tofixedarray( &cur, limit, (FT_Int)max_objects,
                                      temp + i * max_objects, 0 );
            if ( result < 0 || (FT_UInt)result < max_objects )
            {
              FT_ERROR(( "ps_parser_load_field:"
                         " expected %d integers in the %s subarray\n"
                         "                     "
                         " of /FontBBox in the /Blend dictionary\n",
                         max_objects,
                         i == 0 ? "first"
                                : ( i == 1 ? "second"
                                           : ( i == 2 ? "third"
                                                      : "fourth" ) ) ));
              error = FT_THROW( Invalid_File_Format );
              goto Exit;
            }

            skip_spaces( &cur, limit );
          }

          for ( i = 0; i < max_objects; i++ )
          {
            FT_BBox*  bbox = (FT_BBox*)objects[i];


            bbox->xMin = FT_RoundFix( temp[i                  ] );
            bbox->yMin = FT_RoundFix( temp[i +     max_objects] );
            bbox->xMax = FT_RoundFix( temp[i + 2 * max_objects] );
            bbox->yMax = FT_RoundFix( temp[i + 3 * max_objects] );
          }

          FT_FREE( temp );
        }
        break;

      default:
        /* an error occurred */
        goto Fail;
      }
    }

#if 0  /* obsolete -- keep for reference */
    if ( pflags )
      *pflags |= 1L << field->flag_bit;
#else
    FT_UNUSED( pflags );
#endif

    error = FT_Err_Ok;

  Exit:
    return error;

  Fail:
    error = FT_THROW( Invalid_File_Format );
    goto Exit;
  }
