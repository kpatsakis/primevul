  gray_record_cell( RAS_ARG )
  {
    if ( !ras.invalid && ( ras.area | ras.cover ) )
    {
      PCell  cell = gray_find_cell( RAS_VAR );


      cell->area  += ras.area;
      cell->cover += ras.cover;
    }
  }
