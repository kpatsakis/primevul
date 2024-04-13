  tt_cmap14_ensure( TT_CMap14  cmap,
                    FT_UInt32  num_results,
                    FT_Memory  memory )
  {
    FT_UInt32  old_max = cmap->max_results;
    FT_Error   error   = FT_Err_Ok;


    if ( num_results > cmap->max_results )
    {
       cmap->memory = memory;

       if ( FT_QRENEW_ARRAY( cmap->results, old_max, num_results ) )
         return error;

       cmap->max_results = num_results;
    }

    return error;
  }
