  cf2_decoder_parse_charstrings( CFF_Decoder*  decoder,
                                 FT_Byte*      charstring_base,
                                 FT_ULong      charstring_len )
  {
    FT_Memory  memory;
    FT_Error   error = FT_Err_Ok;
    CF2_Font   font;


    FT_ASSERT( decoder && decoder->cff );

    memory = decoder->builder.memory;

    /* CF2 data is saved here across glyphs */
    font = (CF2_Font)decoder->cff->cf2_instance.data;

    /* on first glyph, allocate instance structure */
    if ( decoder->cff->cf2_instance.data == NULL )
    {
      decoder->cff->cf2_instance.finalizer =
        (FT_Generic_Finalizer)cf2_free_instance;

      if ( FT_ALLOC( decoder->cff->cf2_instance.data,
                     sizeof ( CF2_FontRec ) ) )
        return FT_THROW( Out_Of_Memory );

      font = (CF2_Font)decoder->cff->cf2_instance.data;

      font->memory = memory;

      /* initialize a client outline, to be shared by each glyph rendered */
      cf2_outline_init( &font->outline, font->memory, &font->error );
    }

    /* save decoder; it is a stack variable and will be different on each */
    /* call                                                               */
    font->decoder         = decoder;
    font->outline.decoder = decoder;

    {
      /* build parameters for Adobe engine */

      CFF_Builder*  builder = &decoder->builder;
      CFF_Driver    driver  = (CFF_Driver)FT_FACE_DRIVER( builder->face );

      /* local error */
      FT_Error       error2 = FT_Err_Ok;
      CF2_BufferRec  buf;
      CF2_Matrix     transform;
      CF2_F16Dot16   glyphWidth;

      FT_Bool  hinted;
      FT_Bool  scaled;


      /* FreeType has already looked up the GID; convert to         */
      /* `RegionBuffer', assuming that the input has been validated */
      FT_ASSERT( charstring_base + charstring_len >= charstring_base );

      FT_ZERO( &buf );
      buf.start =
      buf.ptr   = charstring_base;
      buf.end   = charstring_base + charstring_len;

      FT_ZERO( &transform );

      cf2_getScaleAndHintFlag( decoder,
                               &transform.a,
                               &transform.d,
                               &hinted,
                               &scaled );

      font->renderingFlags = 0;
      if ( hinted )
        font->renderingFlags |= CF2_FlagsHinted;
      if ( scaled && !driver->no_stem_darkening )
        font->renderingFlags |= CF2_FlagsDarkened;

      font->darkenParams[0] = driver->darken_params[0];
      font->darkenParams[1] = driver->darken_params[1];
      font->darkenParams[2] = driver->darken_params[2];
      font->darkenParams[3] = driver->darken_params[3];
      font->darkenParams[4] = driver->darken_params[4];
      font->darkenParams[5] = driver->darken_params[5];
      font->darkenParams[6] = driver->darken_params[6];
      font->darkenParams[7] = driver->darken_params[7];

      /* now get an outline for this glyph;      */
      /* also get units per em to validate scale */
      font->unitsPerEm = (CF2_Int)cf2_getUnitsPerEm( decoder );

      if ( scaled )
      {
        error2 = cf2_checkTransform( &transform, font->unitsPerEm );
        if ( error2 )
          return error2;
      }

      error2 = cf2_getGlyphOutline( font, &buf, &transform, &glyphWidth );
      if ( error2 )
        return FT_ERR( Invalid_File_Format );

      cf2_setGlyphWidth( &font->outline, glyphWidth );

      return FT_Err_Ok;
    }
  }
