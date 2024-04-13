  T1_Get_Multi_Master( T1_Face           face,
                       FT_Multi_Master*  master )
  {
    PS_Blend  blend = face->blend;
    FT_UInt   n;
    FT_Error  error;


    error = FT_THROW( Invalid_Argument );

    if ( blend )
    {
      master->num_axis    = blend->num_axis;
      master->num_designs = blend->num_designs;

      for ( n = 0; n < blend->num_axis; n++ )
      {
        FT_MM_Axis*   axis = master->axis + n;
        PS_DesignMap  map = blend->design_map + n;


        axis->name    = blend->axis_names[n];
        axis->minimum = map->design_points[0];
        axis->maximum = map->design_points[map->num_points - 1];
      }

      error = FT_Err_Ok;
    }

    return error;
  }
