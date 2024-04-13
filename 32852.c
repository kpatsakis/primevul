  FT_Select_Size( FT_Face  face,
                  FT_Int   strike_index )
  {
    FT_Driver_Class  clazz;


    if ( !face || !FT_HAS_FIXED_SIZES( face ) )
      return FT_Err_Invalid_Face_Handle;

    if ( strike_index < 0 || strike_index >= face->num_fixed_sizes )
      return FT_Err_Invalid_Argument;

    clazz = face->driver->clazz;

    if ( clazz->select_size )
      return clazz->select_size( face->size, (FT_ULong)strike_index );

    FT_Select_Metrics( face, (FT_ULong)strike_index );

    return FT_Err_Ok;
  }
