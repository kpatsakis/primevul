  psh_hint_table_record( PSH_Hint_Table  table,
                         FT_UInt         idx )
  {
    PSH_Hint  hint = table->hints + idx;


    if ( idx >= table->max_hints )
    {
      FT_TRACE0(( "psh_hint_table_record: invalid hint index %d\n", idx ));
      return;
    }

    /* ignore active hints */
    if ( psh_hint_is_active( hint ) )
      return;

    psh_hint_activate( hint );

    /* now scan the current active hint set to check */
    /* whether `hint' overlaps with another hint     */
    {
      PSH_Hint*  sorted = table->sort_global;
      FT_UInt    count  = table->num_hints;
      PSH_Hint   hint2;


      hint->parent = 0;
      for ( ; count > 0; count--, sorted++ )
      {
        hint2 = sorted[0];

        if ( psh_hint_overlap( hint, hint2 ) )
        {
          hint->parent = hint2;
          break;
        }
      }
    }

    if ( table->num_hints < table->max_hints )
      table->sort_global[table->num_hints++] = hint;
    else
      FT_TRACE0(( "psh_hint_table_record: too many sorted hints!  BUG!\n" ));
  }
