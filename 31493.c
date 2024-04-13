  _bdf_is_atom( char*          line,
                unsigned long  linelen,
                char**         name,
                char**         value,
                bdf_font_t*    font )
  {
    int              hold;
    char             *sp, *ep;
    bdf_property_t*  p;


    *name = sp = ep = line;

    while ( *ep && *ep != ' ' && *ep != '\t' )
      ep++;

    hold = -1;
    if ( *ep )
    {
      hold = *ep;
      *ep  = 0;
    }

    p = bdf_get_property( sp, font );

    /* Restore the character that was saved before any return can happen. */
    if ( hold != -1 )
      *ep = (char)hold;

    /* If the property exists and is not an atom, just return here. */
    if ( p && p->format != BDF_ATOM )
      return 0;

    /* The property is an atom.  Trim all leading and trailing whitespace */
    /* and double quotes for the atom value.                              */
    sp = ep;
    ep = line + linelen;

    /* Trim the leading whitespace if it exists. */
    if ( *sp )
      *sp++ = 0;
    while ( *sp                           &&
            ( *sp == ' ' || *sp == '\t' ) )
      sp++;

    /* Trim the leading double quote if it exists. */
    if ( *sp == '"' )
      sp++;
    *value = sp;

    /* Trim the trailing whitespace if it exists. */
    while ( ep > sp                                       &&
            ( *( ep - 1 ) == ' ' || *( ep - 1 ) == '\t' ) )
      *--ep = 0;

    /* Trim the trailing double quote if it exists. */
    if ( ep > sp && *( ep - 1 ) == '"' )
      *--ep = 0;

    return 1;
  }
