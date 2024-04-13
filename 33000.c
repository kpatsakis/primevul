  Ins_GETINFO( TT_ExecContext  exc,
               FT_Long*        args )
  {
    FT_Long    K;
    TT_Driver  driver = (TT_Driver)FT_FACE_DRIVER( exc->face );


    K = 0;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    /********************************/
    /* RASTERIZER VERSION           */
    /* Selector Bit:  0             */
    /* Return Bit(s): 0-7           */
    /*                              */
    if ( SUBPIXEL_HINTING_INFINALITY &&
         ( args[0] & 1 ) != 0        &&
         exc->subpixel_hinting       )
    {
      if ( exc->ignore_x_mode )
      {
        /* if in ClearType backward compatibility mode,         */
        /* we sometimes change the TrueType version dynamically */
        K = exc->rasterizer_version;
        FT_TRACE6(( "Setting rasterizer version %d\n",
                    exc->rasterizer_version ));
      }
      else
        K = TT_INTERPRETER_VERSION_38;
    }
    else
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */
      if ( ( args[0] & 1 ) != 0 )
        K = driver->interpreter_version;

    /********************************/
    /* GLYPH ROTATED                */
    /* Selector Bit:  1             */
    /* Return Bit(s): 8             */
    /*                              */
    if ( ( args[0] & 2 ) != 0 && exc->tt_metrics.rotated )
      K |= 1 << 8;

    /********************************/
    /* GLYPH STRETCHED              */
    /* Selector Bit:  2             */
    /* Return Bit(s): 9             */
    /*                              */
    if ( ( args[0] & 4 ) != 0 && exc->tt_metrics.stretched )
      K |= 1 << 9;

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
    /********************************/
    /* VARIATION GLYPH              */
    /* Selector Bit:  3             */
    /* Return Bit(s): 10            */
    /*                              */
    /* XXX: UNDOCUMENTED!           */
    if ( (args[0] & 8 ) != 0 && exc->face->blend )
      K |= 1 << 10;
#endif

    /********************************/
    /* BI-LEVEL HINTING AND         */
    /* GRAYSCALE RENDERING          */
    /* Selector Bit:  5             */
    /* Return Bit(s): 12            */
    /*                              */
    if ( ( args[0] & 32 ) != 0 && exc->grayscale )
      K |= 1 << 12;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    /* Toggle the following flags only outside of monochrome mode.      */
    /* Otherwise, instructions may behave weirdly and rendering results */
    /* may differ between v35 and v40 mode, e.g., in `Times New Roman   */
    /* Bold Italic'. */
    if ( SUBPIXEL_HINTING_MINIMAL && exc->subpixel_hinting_lean )
    {
      /********************************/
      /* HINTING FOR SUBPIXEL         */
      /* Selector Bit:  6             */
      /* Return Bit(s): 13            */
      /*                              */
      /* v40 does subpixel hinting by default. */
      if ( ( args[0] & 64 ) != 0 )
        K |= 1 << 13;

      /********************************/
      /* VERTICAL LCD SUBPIXELS?      */
      /* Selector Bit:  8             */
      /* Return Bit(s): 15            */
      /*                              */
      if ( ( args[0] & 256 ) != 0 && exc->vertical_lcd_lean )
        K |= 1 << 15;

      /********************************/
      /* SUBPIXEL POSITIONED?         */
      /* Selector Bit:  10            */
      /* Return Bit(s): 17            */
      /*                              */
      /* XXX: FreeType supports it, dependent on what client does? */
      if ( ( args[0] & 1024 ) != 0 )
        K |= 1 << 17;

      /********************************/
      /* SYMMETRICAL SMOOTHING        */
      /* Selector Bit:  11            */
      /* Return Bit(s): 18            */
      /*                              */
      /* The only smoothing method FreeType supports unless someone sets */
      /* FT_LOAD_TARGET_MONO.                                            */
      if ( ( args[0] & 2048 ) != 0 && exc->subpixel_hinting_lean )
        K |= 1 << 18;

      /********************************/
      /* CLEARTYPE HINTING AND        */
      /* GRAYSCALE RENDERING          */
      /* Selector Bit:  12            */
      /* Return Bit(s): 19            */
      /*                              */
      /* Grayscale rendering is what FreeType does anyway unless someone */
      /* sets FT_LOAD_TARGET_MONO or FT_LOAD_TARGET_LCD(_V)              */
      if ( ( args[0] & 4096 ) != 0 && exc->grayscale_cleartype )
        K |= 1 << 19;
    }
#endif

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY

    if ( SUBPIXEL_HINTING_INFINALITY                          &&
         exc->rasterizer_version >= TT_INTERPRETER_VERSION_35 )
    {

      if ( exc->rasterizer_version >= 37 )
      {
        /********************************/
        /* HINTING FOR SUBPIXEL         */
        /* Selector Bit:  6             */
        /* Return Bit(s): 13            */
        /*                              */
        if ( ( args[0] & 64 ) != 0 && exc->subpixel_hinting )
          K |= 1 << 13;

        /********************************/
        /* COMPATIBLE WIDTHS ENABLED    */
        /* Selector Bit:  7             */
        /* Return Bit(s): 14            */
        /*                              */
        /* Functionality still needs to be added */
        if ( ( args[0] & 128 ) != 0 && exc->compatible_widths )
          K |= 1 << 14;

        /********************************/
        /* VERTICAL LCD SUBPIXELS?      */
        /* Selector Bit:  8             */
        /* Return Bit(s): 15            */
        /*                              */
        /* Functionality still needs to be added */
        if ( ( args[0] & 256 ) != 0 && exc->vertical_lcd )
          K |= 1 << 15;

        /********************************/
        /* HINTING FOR BGR?             */
        /* Selector Bit:  9             */
        /* Return Bit(s): 16            */
        /*                              */
        /* Functionality still needs to be added */
        if ( ( args[0] & 512 ) != 0 && exc->bgr )
          K |= 1 << 16;

        if ( exc->rasterizer_version >= 38 )
        {
          /********************************/
          /* SUBPIXEL POSITIONED?         */
          /* Selector Bit:  10            */
          /* Return Bit(s): 17            */
          /*                              */
          /* Functionality still needs to be added */
          if ( ( args[0] & 1024 ) != 0 && exc->subpixel_positioned )
            K |= 1 << 17;

          /********************************/
          /* SYMMETRICAL SMOOTHING        */
          /* Selector Bit:  11            */
          /* Return Bit(s): 18            */
          /*                              */
          /* Functionality still needs to be added */
          if ( ( args[0] & 2048 ) != 0 && exc->symmetrical_smoothing )
            K |= 1 << 18;

          /********************************/
          /* GRAY CLEARTYPE               */
          /* Selector Bit:  12            */
          /* Return Bit(s): 19            */
          /*                              */
          /* Functionality still needs to be added */
          if ( ( args[0] & 4096 ) != 0 && exc->gray_cleartype )
            K |= 1 << 19;
        }
      }
    }

#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    args[0] = K;
  }
