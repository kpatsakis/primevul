  _bdf_atoul( char*   s,
              char**  end,
              int     base )
  {
    unsigned long         v;
    const unsigned char*  dmap;


    if ( s == 0 || *s == 0 )
      return 0;

    /* Make sure the radix is something recognizable.  Default to 10. */
    switch ( base )
    {
    case 8:
      dmap = odigits;
      break;
    case 16:
      dmap = hdigits;
      break;
    default:
      base = 10;
      dmap = ddigits;
      break;
    }

    /* Check for the special hex prefix. */
    if ( *s == '0'                                  &&
         ( *( s + 1 ) == 'x' || *( s + 1 ) == 'X' ) )
    {
      base = 16;
      dmap = hdigits;
      s   += 2;
    }

    for ( v = 0; sbitset( dmap, *s ); s++ )
      v = v * base + a2i[(int)*s];

    if ( end != 0 )
      *end = s;

    return v;
  }
