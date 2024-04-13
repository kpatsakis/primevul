  cff_parse_num( CFF_Parser  parser,
                 FT_Byte**   d )
  {
    if ( **d == 30 )
    {
      /* binary-coded decimal is truncated to integer */
      return cff_parse_real( *d, parser->limit, 0, NULL ) >> 16;
    }

    else if ( **d == 255 )
    {
      /* 16.16 fixed point is used internally for CFF2 blend results. */
      /* Since these are trusted values, a limit check is not needed. */

      /* After the 255, 4 bytes are in host order. */
      /* Blend result is rounded to integer.       */
      return (FT_Long)( *( (FT_UInt32 *) ( d[0] + 1 ) ) + 0x8000U ) >> 16;
    }

    else
      return cff_parse_integer( *d, parser->limit );
  }
