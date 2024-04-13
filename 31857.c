  skip_comment( FT_Byte*  *acur,
                FT_Byte*   limit )
  {
    FT_Byte*  cur = *acur;


    while ( cur < limit )
    {
      if ( IS_PS_NEWLINE( *cur ) )
        break;
      cur++;
    }

    *acur = cur;
  }
