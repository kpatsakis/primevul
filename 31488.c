  _bdf_add_comment( bdf_font_t*    font,
                    char*          comment,
                    unsigned long  len )
  {
    char*      cp;
    FT_Memory  memory = font->memory;
    FT_Error   error = BDF_Err_Ok;


    if ( FT_RENEW_ARRAY( font->comments,
                         font->comments_len,
                         font->comments_len + len + 1 ) )
      goto Exit;

    cp = font->comments + font->comments_len;

    FT_MEM_COPY( cp, comment, len );
    cp[len] = '\n';

    font->comments_len += len + 1;

  Exit:
    return error;
  }
