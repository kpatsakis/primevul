  do_fixed( CFF_Parser  parser,
            FT_Byte**   d,
            FT_Long     scaling )
  {
    if ( **d == 30 )
      return cff_parse_real( *d, parser->limit, scaling, NULL );
    else
    {
      FT_Long  val = cff_parse_integer( *d, parser->limit );


      if ( scaling )
        val *= power_tens[scaling];

      if ( val > 0x7FFF )
      {
        val = 0x7FFFFFFFL;
        goto Overflow;
      }
      else if ( val < -0x7FFF )
      {
        val = -0x7FFFFFFFL;
        goto Overflow;
      }

      return (FT_Long)( (FT_ULong)val << 16 );

    Overflow:
      FT_TRACE4(( "!!!OVERFLOW:!!!" ));
      return val;
    }
  }
