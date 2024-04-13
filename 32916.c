  psh_hint_table_align_hints( PSH_Hint_Table  table,
                              PSH_Globals     globals,
                              FT_Int          dimension,
                              PSH_Glyph       glyph )
  {
    PSH_Hint       hint;
    FT_UInt        count;

#ifdef DEBUG_HINTER

    PSH_Dimension  dim   = &globals->dimension[dimension];
    FT_Fixed       scale = dim->scale_mult;
    FT_Fixed       delta = dim->scale_delta;


    if ( ps_debug_no_vert_hints && dimension == 0 )
    {
      ps_simple_scale( table, scale, delta, dimension );
      return;
    }

    if ( ps_debug_no_horz_hints && dimension == 1 )
    {
      ps_simple_scale( table, scale, delta, dimension );
      return;
    }

#endif /* DEBUG_HINTER*/

    hint  = table->hints;
    count = table->max_hints;

    for ( ; count > 0; count--, hint++ )
      psh_hint_align( hint, globals, dimension, glyph );
  }
