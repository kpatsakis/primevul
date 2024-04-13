  read_binary_data( T1_Parser  parser,
                    FT_Long*   size,
                    FT_Byte**  base,
                    FT_Bool    incremental )
  {
    FT_Byte*  cur;
    FT_Byte*  limit = parser->root.limit;


    /* the binary data has one of the following formats */
    /*                                                  */
    /*   `size' [white*] RD white ....... ND            */
    /*   `size' [white*] -| white ....... |-            */
    /*                                                  */

    T1_Skip_Spaces( parser );

    cur = parser->root.cursor;

    if ( cur < limit && ft_isdigit( *cur ) )
    {
      FT_Long  s = T1_ToInt( parser );


      T1_Skip_PS_Token( parser );   /* `RD' or `-|' or something else */

      /* there is only one whitespace char after the */
      /* `RD' or `-|' token                          */
      *base = parser->root.cursor + 1;

      if ( s >= 0 && s < limit - *base )
      {
        parser->root.cursor += s + 1;
        *size = s;
        return !parser->root.error;
      }
    }

    if( !incremental )
    {
      FT_ERROR(( "read_binary_data: invalid size field\n" ));
      parser->root.error = FT_THROW( Invalid_File_Format );
    }

    return 0;
  }
