  skip_literal_string( FT_Byte*  *acur,
                       FT_Byte*   limit )
  {
    FT_Byte*      cur   = *acur;
    FT_Int        embed = 0;
    FT_Error      error = FT_ERR( Invalid_File_Format );
    unsigned int  i;


    while ( cur < limit )
    {
      FT_Byte  c = *cur;


      cur++;

      if ( c == '\\' )
      {
        /* Red Book 3rd ed., section `Literal Text Strings', p. 29:     */
        /* A backslash can introduce three different types              */
        /* of escape sequences:                                         */
        /*   - a special escaped char like \r, \n, etc.                 */
        /*   - a one-, two-, or three-digit octal number                */
        /*   - none of the above in which case the backslash is ignored */

        if ( cur == limit )
          /* error (or to be ignored?) */
          break;

        switch ( *cur )
        {
          /* skip `special' escape */
        case 'n':
        case 'r':
        case 't':
        case 'b':
        case 'f':
        case '\\':
        case '(':
        case ')':
          cur++;
          break;

        default:
          /* skip octal escape or ignore backslash */
          for ( i = 0; i < 3 && cur < limit; i++ )
          {
            if ( !IS_OCTAL_DIGIT( *cur ) )
              break;

            cur++;
          }
        }
      }
      else if ( c == '(' )
        embed++;
      else if ( c == ')' )
      {
        embed--;
        if ( embed == 0 )
        {
          error = FT_Err_Ok;
          break;
        }
      }
    }

    *acur = cur;

    return error;
  }
