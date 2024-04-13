  ps_simple_scale( PSH_Hint_Table  table,
                   FT_Fixed        scale,
                   FT_Fixed        delta,
                   FT_Int          dimension )
  {
    PSH_Hint  hint;
    FT_UInt   count;


    for ( count = 0; count < table->max_hints; count++ )
    {
      hint = table->hints + count;

      hint->cur_pos = FT_MulFix( hint->org_pos, scale ) + delta;
      hint->cur_len = FT_MulFix( hint->org_len, scale );

      if ( ps_debug_hint_func )
        ps_debug_hint_func( hint, dimension );
    }
  }
