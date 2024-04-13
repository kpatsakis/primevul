  ft_var_apply_tuple( GX_Blend   blend,
                      FT_UShort  tupleIndex,
                      FT_Fixed*  tuple_coords,
                      FT_Fixed*  im_start_coords,
                      FT_Fixed*  im_end_coords )
  {
    FT_UInt   i;
    FT_Fixed  apply;
    FT_Fixed  temp;


    apply = 0x10000L;
    for ( i = 0; i < blend->num_axis; ++i )
    {
      if ( tuple_coords[i] == 0 )
        /* It's not clear why (for intermediate tuples) we don't need     */
        /* to check against start/end -- the documentation says we don't. */
        /* Similarly, it's unclear why we don't need to scale along the   */
        /* axis.                                                          */
        continue;

      else if ( blend->normalizedcoords[i] == 0                           ||
                ( blend->normalizedcoords[i] < 0 && tuple_coords[i] > 0 ) ||
                ( blend->normalizedcoords[i] > 0 && tuple_coords[i] < 0 ) )
      {
        apply = 0;
        break;
      }

      else if ( !( tupleIndex & GX_TI_INTERMEDIATE_TUPLE ) )
        /* not an intermediate tuple */
        apply = FT_MulDiv( apply,
                           blend->normalizedcoords[i] > 0
                             ? blend->normalizedcoords[i]
                             : -blend->normalizedcoords[i],
                           0x10000L );

      else if ( blend->normalizedcoords[i] <= im_start_coords[i] ||
                blend->normalizedcoords[i] >= im_end_coords[i]   )
      {
        apply = 0;
        break;
      }

      else if ( blend->normalizedcoords[i] < tuple_coords[i] )
      {
        temp = FT_MulDiv( blend->normalizedcoords[i] - im_start_coords[i],
                          0x10000L,
                          tuple_coords[i] - im_start_coords[i]);
        apply = FT_MulDiv( apply, temp, 0x10000L );
      }

      else
      {
        temp = FT_MulDiv( im_end_coords[i] - blend->normalizedcoords[i],
                          0x10000L,
                          im_end_coords[i] - tuple_coords[i] );
        apply = FT_MulDiv( apply, temp, 0x10000L );
      }
    }

    return apply;
  }
