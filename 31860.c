  t1_decoder_init( T1_Decoder           decoder,
                   FT_Face              face,
                   FT_Size              size,
                   FT_GlyphSlot         slot,
                   FT_Byte**            glyph_names,
                   PS_Blend             blend,
                   FT_Bool              hinting,
                   FT_Render_Mode       hint_mode,
                   T1_Decoder_Callback  parse_callback )
  {
    FT_ZERO( decoder );

    /* retrieve PSNames interface from list of current modules */
    {
      FT_Service_PsCMaps  psnames;


      FT_FACE_FIND_GLOBAL_SERVICE( face, psnames, POSTSCRIPT_CMAPS );
      if ( !psnames )
      {
        FT_ERROR(( "t1_decoder_init:"
                   " the `psnames' module is not available\n" ));
        return FT_THROW( Unimplemented_Feature );
      }

      decoder->psnames = psnames;
    }

    t1_builder_init( &decoder->builder, face, size, slot, hinting );

    /* decoder->buildchar and decoder->len_buildchar have to be  */
    /* initialized by the caller since we cannot know the length */
    /* of the BuildCharArray                                     */

    decoder->num_glyphs     = (FT_UInt)face->num_glyphs;
    decoder->glyph_names    = glyph_names;
    decoder->hint_mode      = hint_mode;
    decoder->blend          = blend;
    decoder->parse_callback = parse_callback;

    decoder->funcs          = t1_decoder_funcs;

    return FT_Err_Ok;
  }
