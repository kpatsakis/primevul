  _bdf_list_split( _bdf_list_t*   list,
                   char*          separators,
                   char*          line,
                   unsigned long  linelen )
  {
    int       mult, final_empty;
    char      *sp, *ep, *end;
    char      seps[32];
    FT_Error  error = BDF_Err_Ok;


    /* Initialize the list. */
    list->used = 0;
    if ( list->size )
    {
      list->field[0] = (char*)empty;
      list->field[1] = (char*)empty;
      list->field[2] = (char*)empty;
      list->field[3] = (char*)empty;
      list->field[4] = (char*)empty;
    }

    /* If the line is empty, then simply return. */
    if ( linelen == 0 || line[0] == 0 )
      goto Exit;

    /* In the original code, if the `separators' parameter is NULL or */
    /* empty, the list is split into individual bytes.  We don't need */
    /* this, so an error is signaled.                                 */
    if ( separators == 0 || *separators == 0 )
    {
      error = BDF_Err_Invalid_Argument;
      goto Exit;
    }

    /* Prepare the separator bitmap. */
    FT_MEM_ZERO( seps, 32 );

    /* If the very last character of the separator string is a plus, then */
    /* set the `mult' flag to indicate that multiple separators should be */
    /* collapsed into one.                                                */
    for ( mult = 0, sp = separators; sp && *sp; sp++ )
    {
      if ( *sp == '+' && *( sp + 1 ) == 0 )
        mult = 1;
      else
        setsbit( seps, *sp );
    }

    /* Break the line up into fields. */
    for ( final_empty = 0, sp = ep = line, end = sp + linelen;
          sp < end && *sp; )
    {
      /* Collect everything that is not a separator. */
      for ( ; *ep && !sbitset( seps, *ep ); ep++ )
        ;

      /* Resize the list if necessary. */
      if ( list->used == list->size )
      {
        error = _bdf_list_ensure( list, list->used + 1 );
        if ( error )
          goto Exit;
      }

      /* Assign the field appropriately. */
      list->field[list->used++] = ( ep > sp ) ? sp : (char*)empty;

      sp = ep;

      if ( mult )
      {
        /* If multiple separators should be collapsed, do it now by */
        /* setting all the separator characters to 0.               */
        for ( ; *ep && sbitset( seps, *ep ); ep++ )
          *ep = 0;
      }
      else if ( *ep != 0 )
        /* Don't collapse multiple separators by making them 0, so just */
        /* make the one encountered 0.                                  */
        *ep++ = 0;

      final_empty = ( ep > sp && *ep == 0 );
      sp = ep;
    }

    /* Finally, NULL-terminate the list. */
    if ( list->used + final_empty >= list->size )
    {
      error = _bdf_list_ensure( list, list->used + final_empty + 1 );
      if ( error )
        goto Exit;
    }

    if ( final_empty )
      list->field[list->used++] = (char*)empty;

    list->field[list->used] = 0;

  Exit:
    return error;
  }
