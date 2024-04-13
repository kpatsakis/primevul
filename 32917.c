  psh_hint_table_deactivate( PSH_Hint_Table  table )
  {
    FT_UInt   count = table->max_hints;
    PSH_Hint  hint  = table->hints;


    for ( ; count > 0; count--, hint++ )
    {
      psh_hint_deactivate( hint );
      hint->order = -1;
    }
  }
