  cf2_getPpemY( CFF_Decoder*  decoder )
  {
    FT_ASSERT( decoder                          &&
               decoder->builder.face            &&
               decoder->builder.face->root.size );

    /*
     * Note that `y_ppem' can be zero if there wasn't a call to
     * `FT_Set_Char_Size' or something similar.  However, this isn't a
     * problem since we come to this place in the code only if
     * FT_LOAD_NO_SCALE is set (the other case gets caught by
     * `cf2_checkTransform').  The ppem value is needed to compute the stem
     * darkening, which is disabled for getting the unscaled outline.
     *
     */
    return cf2_intToFixed(
             decoder->builder.face->root.size->metrics.y_ppem );
  }
