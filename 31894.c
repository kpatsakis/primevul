  sfnt_load_face( FT_Stream      stream,
                  TT_Face        face,
                  FT_Int         face_instance_index,
                  FT_Int         num_params,
                  FT_Parameter*  params )
  {
    FT_Error      error;
#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
    FT_Error      psnames_error;
#endif
    FT_Bool       has_outline;
    FT_Bool       is_apple_sbit;
    FT_Bool       is_apple_sbix;
    FT_Bool       ignore_typographic_family    = FALSE;
    FT_Bool       ignore_typographic_subfamily = FALSE;

    SFNT_Service  sfnt = (SFNT_Service)face->sfnt;

    FT_UNUSED( face_instance_index );


    /* Check parameters */

    {
      FT_Int  i;


      for ( i = 0; i < num_params; i++ )
      {
        if ( params[i].tag == FT_PARAM_TAG_IGNORE_TYPOGRAPHIC_FAMILY )
          ignore_typographic_family = TRUE;
        else if ( params[i].tag == FT_PARAM_TAG_IGNORE_TYPOGRAPHIC_SUBFAMILY )
          ignore_typographic_subfamily = TRUE;
      }
    }

    /* Load tables */

    /* We now support two SFNT-based bitmapped font formats.  They */
    /* are recognized easily as they do not include a `glyf'       */
    /* table.                                                      */
    /*                                                             */
    /* The first format comes from Apple, and uses a table named   */
    /* `bhed' instead of `head' to store the font header (using    */
    /* the same format).  It also doesn't include horizontal and   */
    /* vertical metrics tables (i.e. `hhea' and `vhea' tables are  */
    /* missing).                                                   */
    /*                                                             */
    /* The other format comes from Microsoft, and is used with     */
    /* WinCE/PocketPC.  It looks like a standard TTF, except that  */
    /* it doesn't contain outlines.                                */
    /*                                                             */

    FT_TRACE2(( "sfnt_load_face: %08p\n\n", face ));

    /* do we have outlines in there? */
#ifdef FT_CONFIG_OPTION_INCREMENTAL
    has_outline = FT_BOOL( face->root.internal->incremental_interface ||
                           tt_face_lookup_table( face, TTAG_glyf )    ||
                           tt_face_lookup_table( face, TTAG_CFF )     ||
                           tt_face_lookup_table( face, TTAG_CFF2 )    );
#else
    has_outline = FT_BOOL( tt_face_lookup_table( face, TTAG_glyf ) ||
                           tt_face_lookup_table( face, TTAG_CFF )  ||
                           tt_face_lookup_table( face, TTAG_CFF2 ) );
#endif

    is_apple_sbit = 0;
    is_apple_sbix = !face->goto_table( face, TTAG_sbix, stream, 0 );

    /* Apple 'sbix' color bitmaps are rendered scaled and then the 'glyf'
     * outline rendered on top.  We don't support that yet, so just ignore
     * the 'glyf' outline and advertise it as a bitmap-only font. */
    if ( is_apple_sbix )
      has_outline = FALSE;

    /* if this font doesn't contain outlines, we try to load */
    /* a `bhed' table                                        */
    if ( !has_outline && sfnt->load_bhed )
    {
      LOAD_( bhed );
      is_apple_sbit = FT_BOOL( !error );
    }

    /* load the font header (`head' table) if this isn't an Apple */
    /* sbit font file                                             */
    if ( !is_apple_sbit || is_apple_sbix )
    {
      LOAD_( head );
      if ( error )
        goto Exit;
    }

    if ( face->header.Units_Per_EM == 0 )
    {
      error = FT_THROW( Invalid_Table );

      goto Exit;
    }

    /* the following tables are often not present in embedded TrueType */
    /* fonts within PDF documents, so don't check for them.            */
    LOAD_( maxp );
    LOAD_( cmap );

    /* the following tables are optional in PCL fonts -- */
    /* don't check for errors                            */
    LOAD_( name );
    LOAD_( post );

#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
    psnames_error = error;
#endif

    /* do not load the metrics headers and tables if this is an Apple */
    /* sbit font file                                                 */
    if ( !is_apple_sbit )
    {
      /* load the `hhea' and `hmtx' tables */
      LOADM_( hhea, 0 );
      if ( !error )
      {
        LOADM_( hmtx, 0 );
        if ( FT_ERR_EQ( error, Table_Missing ) )
        {
          error = FT_THROW( Hmtx_Table_Missing );

#ifdef FT_CONFIG_OPTION_INCREMENTAL
          /* If this is an incrementally loaded font and there are */
          /* overriding metrics, tolerate a missing `hmtx' table.  */
          if ( face->root.internal->incremental_interface          &&
               face->root.internal->incremental_interface->funcs->
                 get_glyph_metrics                                 )
          {
            face->horizontal.number_Of_HMetrics = 0;
            error                               = FT_Err_Ok;
          }
#endif
        }
      }
      else if ( FT_ERR_EQ( error, Table_Missing ) )
      {
        /* No `hhea' table necessary for SFNT Mac fonts. */
        if ( face->format_tag == TTAG_true )
        {
          FT_TRACE2(( "This is an SFNT Mac font.\n" ));

          has_outline = 0;
          error       = FT_Err_Ok;
        }
        else
        {
          error = FT_THROW( Horiz_Header_Missing );

#ifdef FT_CONFIG_OPTION_INCREMENTAL
          /* If this is an incrementally loaded font and there are */
          /* overriding metrics, tolerate a missing `hhea' table.  */
          if ( face->root.internal->incremental_interface          &&
               face->root.internal->incremental_interface->funcs->
                 get_glyph_metrics                                 )
          {
            face->horizontal.number_Of_HMetrics = 0;
            error                               = FT_Err_Ok;
          }
#endif

        }
      }

      if ( error )
        goto Exit;

      /* try to load the `vhea' and `vmtx' tables */
      LOADM_( hhea, 1 );
      if ( !error )
      {
        LOADM_( hmtx, 1 );
        if ( !error )
          face->vertical_info = 1;
      }

      if ( error && FT_ERR_NEQ( error, Table_Missing ) )
        goto Exit;

      LOAD_( os2 );
      if ( error )
      {
        /* we treat the table as missing if there are any errors */
        face->os2.version = 0xFFFFU;
      }
    }

    /* the optional tables */

    /* embedded bitmap support */
    if ( sfnt->load_eblc )
      LOAD_( eblc );

    /* consider the pclt, kerning, and gasp tables as optional */
    LOAD_( pclt );
    LOAD_( gasp );
    LOAD_( kern );

    face->root.num_glyphs = face->max_profile.numGlyphs;

    /* Bit 8 of the `fsSelection' field in the `OS/2' table denotes  */
    /* a WWS-only font face.  `WWS' stands for `weight', width', and */
    /* `slope', a term used by Microsoft's Windows Presentation      */
    /* Foundation (WPF).  This flag has been introduced in version   */
    /* 1.5 of the OpenType specification (May 2008).                 */

    face->root.family_name = NULL;
    face->root.style_name  = NULL;
    if ( face->os2.version != 0xFFFFU && face->os2.fsSelection & 256 )
    {
      if ( !ignore_typographic_family )
        GET_NAME( TYPOGRAPHIC_FAMILY, &face->root.family_name );
      if ( !face->root.family_name )
        GET_NAME( FONT_FAMILY, &face->root.family_name );

      if ( !ignore_typographic_subfamily )
        GET_NAME( TYPOGRAPHIC_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name )
        GET_NAME( FONT_SUBFAMILY, &face->root.style_name );
    }
    else
    {
      GET_NAME( WWS_FAMILY, &face->root.family_name );
      if ( !face->root.family_name && !ignore_typographic_family )
        GET_NAME( TYPOGRAPHIC_FAMILY, &face->root.family_name );
      if ( !face->root.family_name )
        GET_NAME( FONT_FAMILY, &face->root.family_name );

      GET_NAME( WWS_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name && !ignore_typographic_subfamily )
        GET_NAME( TYPOGRAPHIC_SUBFAMILY, &face->root.style_name );
      if ( !face->root.style_name )
        GET_NAME( FONT_SUBFAMILY, &face->root.style_name );
    }

    /* now set up root fields */
    {
      FT_Face  root  = &face->root;
      FT_Long  flags = root->face_flags;


      /*********************************************************************/
      /*                                                                   */
      /* Compute face flags.                                               */
      /*                                                                   */
      if ( face->sbit_table_type == TT_SBIT_TABLE_TYPE_CBLC ||
           face->sbit_table_type == TT_SBIT_TABLE_TYPE_SBIX )
        flags |= FT_FACE_FLAG_COLOR;      /* color glyphs */

      if ( has_outline == TRUE )
        flags |= FT_FACE_FLAG_SCALABLE;   /* scalable outlines */

      /* The sfnt driver only supports bitmap fonts natively, thus we */
      /* don't set FT_FACE_FLAG_HINTER.                               */
      flags |= FT_FACE_FLAG_SFNT       |  /* SFNT file format  */
               FT_FACE_FLAG_HORIZONTAL;   /* horizontal data   */

#ifdef TT_CONFIG_OPTION_POSTSCRIPT_NAMES
      if ( !psnames_error                             &&
           face->postscript.FormatType != 0x00030000L )
        flags |= FT_FACE_FLAG_GLYPH_NAMES;
#endif

      /* fixed width font? */
      if ( face->postscript.isFixedPitch )
        flags |= FT_FACE_FLAG_FIXED_WIDTH;

      /* vertical information? */
      if ( face->vertical_info )
        flags |= FT_FACE_FLAG_VERTICAL;

      /* kerning available ? */
      if ( TT_FACE_HAS_KERNING( face ) )
        flags |= FT_FACE_FLAG_KERNING;

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
      /* Don't bother to load the tables unless somebody asks for them. */
      /* No need to do work which will (probably) not be used.          */
      if ( face->variation_support & TT_FACE_FLAG_VAR_FVAR )
      {
        if ( tt_face_lookup_table( face, TTAG_glyf ) != 0 &&
             tt_face_lookup_table( face, TTAG_gvar ) != 0 )
          flags |= FT_FACE_FLAG_MULTIPLE_MASTERS;
        if ( tt_face_lookup_table( face, TTAG_CFF2 ) != 0 )
          flags |= FT_FACE_FLAG_MULTIPLE_MASTERS;
      }
#endif

      root->face_flags = flags;

      /*********************************************************************/
      /*                                                                   */
      /* Compute style flags.                                              */
      /*                                                                   */

      flags = 0;
      if ( has_outline == TRUE && face->os2.version != 0xFFFFU )
      {
        /* We have an OS/2 table; use the `fsSelection' field.  Bit 9 */
        /* indicates an oblique font face.  This flag has been        */
        /* introduced in version 1.5 of the OpenType specification.   */

        if ( face->os2.fsSelection & 512 )       /* bit 9 */
          flags |= FT_STYLE_FLAG_ITALIC;
        else if ( face->os2.fsSelection & 1 )    /* bit 0 */
          flags |= FT_STYLE_FLAG_ITALIC;

        if ( face->os2.fsSelection & 32 )        /* bit 5 */
          flags |= FT_STYLE_FLAG_BOLD;
      }
      else
      {
        /* this is an old Mac font, use the header field */

        if ( face->header.Mac_Style & 1 )
          flags |= FT_STYLE_FLAG_BOLD;

        if ( face->header.Mac_Style & 2 )
          flags |= FT_STYLE_FLAG_ITALIC;
      }

      root->style_flags |= flags;

      /*********************************************************************/
      /*                                                                   */
      /* Polish the charmaps.                                              */
      /*                                                                   */
      /*   Try to set the charmap encoding according to the platform &     */
      /*   encoding ID of each charmap.                                    */
      /*                                                                   */

      tt_face_build_cmaps( face );  /* ignore errors */


      /* set the encoding fields */
      {
        FT_Int  m;


        for ( m = 0; m < root->num_charmaps; m++ )
        {
          FT_CharMap  charmap = root->charmaps[m];


          charmap->encoding = sfnt_find_encoding( charmap->platform_id,
                                                  charmap->encoding_id );

#if 0
          if ( !root->charmap                           &&
               charmap->encoding == FT_ENCODING_UNICODE )
          {
            /* set 'root->charmap' to the first Unicode encoding we find */
            root->charmap = charmap;
          }
#endif
        }
      }

#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS

      /*
       *  Now allocate the root array of FT_Bitmap_Size records and
       *  populate them.  Unfortunately, it isn't possible to indicate bit
       *  depths in the FT_Bitmap_Size record.  This is a design error.
       */
      {
        FT_UInt  count;


        count = face->sbit_num_strikes;

        if ( count > 0 )
        {
          FT_Memory        memory   = face->root.stream->memory;
          FT_UShort        em_size  = face->header.Units_Per_EM;
          FT_Short         avgwidth = face->os2.xAvgCharWidth;
          FT_Size_Metrics  metrics;

          FT_UInt*  sbit_strike_map = NULL;
          FT_UInt   strike_idx, bsize_idx;


          if ( em_size == 0 || face->os2.version == 0xFFFFU )
          {
            avgwidth = 1;
            em_size = 1;
          }

          /* to avoid invalid strike data in the `available_sizes' field */
          /* of `FT_Face', we map `available_sizes' indices to strike    */
          /* indices                                                     */
          if ( FT_NEW_ARRAY( root->available_sizes, count ) ||
               FT_NEW_ARRAY( sbit_strike_map, count ) )
            goto Exit;

          bsize_idx = 0;
          for ( strike_idx = 0; strike_idx < count; strike_idx++ )
          {
            FT_Bitmap_Size*  bsize = root->available_sizes + bsize_idx;


            error = sfnt->load_strike_metrics( face, strike_idx, &metrics );
            if ( error )
              continue;

            bsize->height = (FT_Short)( metrics.height >> 6 );
            bsize->width  = (FT_Short)(
              ( avgwidth * metrics.x_ppem + em_size / 2 ) / em_size );

            bsize->x_ppem = metrics.x_ppem << 6;
            bsize->y_ppem = metrics.y_ppem << 6;

            /* assume 72dpi */
            bsize->size   = metrics.y_ppem << 6;

            /* only use strikes with valid PPEM values */
            if ( bsize->x_ppem && bsize->y_ppem )
              sbit_strike_map[bsize_idx++] = strike_idx;
          }

          /* reduce array size to the actually used elements */
          (void)FT_RENEW_ARRAY( sbit_strike_map, count, bsize_idx );

          /* from now on, all strike indices are mapped */
          /* using `sbit_strike_map'                    */
          if ( bsize_idx )
          {
            face->sbit_strike_map = sbit_strike_map;

            root->face_flags     |= FT_FACE_FLAG_FIXED_SIZES;
            root->num_fixed_sizes = (FT_Int)bsize_idx;
          }
        }
      }

#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */

      /* a font with no bitmaps and no outlines is scalable; */
      /* it has only empty glyphs then                       */
      if ( !FT_HAS_FIXED_SIZES( root ) && !FT_IS_SCALABLE( root ) )
        root->face_flags |= FT_FACE_FLAG_SCALABLE;


      /*********************************************************************/
      /*                                                                   */
      /*  Set up metrics.                                                  */
      /*                                                                   */
      if ( FT_IS_SCALABLE( root ) )
      {
        /* XXX What about if outline header is missing */
        /*     (e.g. sfnt wrapped bitmap)?             */
        root->bbox.xMin    = face->header.xMin;
        root->bbox.yMin    = face->header.yMin;
        root->bbox.xMax    = face->header.xMax;
        root->bbox.yMax    = face->header.yMax;
        root->units_per_EM = face->header.Units_Per_EM;


        /* XXX: Computing the ascender/descender/height is very different */
        /*      from what the specification tells you.  Apparently, we    */
        /*      must be careful because                                   */
        /*                                                                */
        /*      - not all fonts have an OS/2 table; in this case, we take */
        /*        the values in the horizontal header.  However, these    */
        /*        values very often are not reliable.                     */
        /*                                                                */
        /*      - otherwise, the correct typographic values are in the    */
        /*        sTypoAscender, sTypoDescender & sTypoLineGap fields.    */
        /*                                                                */
        /*        However, certain fonts have these fields set to 0.      */
        /*        Rather, they have usWinAscent & usWinDescent correctly  */
        /*        set (but with different values).                        */
        /*                                                                */
        /*      As an example, Arial Narrow is implemented through four   */
        /*      files ARIALN.TTF, ARIALNI.TTF, ARIALNB.TTF & ARIALNBI.TTF */
        /*                                                                */
        /*      Strangely, all fonts have the same values in their        */
        /*      sTypoXXX fields, except ARIALNB which sets them to 0.     */
        /*                                                                */
        /*      On the other hand, they all have different                */
        /*      usWinAscent/Descent values -- as a conclusion, the OS/2   */
        /*      table cannot be used to compute the text height reliably! */
        /*                                                                */

        /* The ascender and descender are taken from the `hhea' table. */
        /* If zero, they are taken from the `OS/2' table.              */

        root->ascender  = face->horizontal.Ascender;
        root->descender = face->horizontal.Descender;

        root->height = root->ascender - root->descender +
                       face->horizontal.Line_Gap;

        if ( !( root->ascender || root->descender ) )
        {
          if ( face->os2.version != 0xFFFFU )
          {
            if ( face->os2.sTypoAscender || face->os2.sTypoDescender )
            {
              root->ascender  = face->os2.sTypoAscender;
              root->descender = face->os2.sTypoDescender;

              root->height = root->ascender - root->descender +
                             face->os2.sTypoLineGap;
            }
            else
            {
              root->ascender  =  (FT_Short)face->os2.usWinAscent;
              root->descender = -(FT_Short)face->os2.usWinDescent;

              root->height = root->ascender - root->descender;
            }
          }
        }

        root->max_advance_width  =
          (FT_Short)face->horizontal.advance_Width_Max;
        root->max_advance_height =
          (FT_Short)( face->vertical_info ? face->vertical.advance_Height_Max
                                          : root->height );

        /* See http://www.microsoft.com/OpenType/OTSpec/post.htm -- */
        /* Adjust underline position from top edge to centre of     */
        /* stroke to convert TrueType meaning to FreeType meaning.  */
        root->underline_position  = face->postscript.underlinePosition -
                                    face->postscript.underlineThickness / 2;
        root->underline_thickness = face->postscript.underlineThickness;
      }

    }

  Exit:
    FT_TRACE2(( "sfnt_load_face: done\n" ));

    return error;
  }
