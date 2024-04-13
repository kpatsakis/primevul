  T1_Set_Var_Design( T1_Face    face,
                     FT_UInt    num_coords,
                     FT_Fixed*  coords )
  {
     FT_Long   lcoords[4];          /* maximum axis count is 4 */
     FT_UInt   i;
     FT_Error  error;


     error = FT_ERR( Invalid_Argument );
     if ( num_coords <= 4 && num_coords > 0 )
     {
       for ( i = 0; i < num_coords; ++i )
         lcoords[i] = FIXED_TO_INT( coords[i] );
       error = T1_Set_MM_Design( face, num_coords, lcoords );
     }

     return error;
  }
