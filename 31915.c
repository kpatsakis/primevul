  tt_done_blend( FT_Memory  memory,
                 GX_Blend   blend )
  {
    if ( blend != NULL )
    {
      FT_UInt  i;


      FT_FREE( blend->normalizedcoords );
      FT_FREE( blend->mmvar );

      if ( blend->avar_segment != NULL )
      {
        for ( i = 0; i < blend->num_axis; ++i )
          FT_FREE( blend->avar_segment[i].correspondence );
        FT_FREE( blend->avar_segment );
      }

      FT_FREE( blend->tuplecoords );
      FT_FREE( blend->glyphoffsets );
      FT_FREE( blend );
    }
  }
