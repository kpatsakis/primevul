  shift_elements( PS_Table  table,
                  FT_Byte*  old_base )
  {
    FT_PtrDist  delta  = table->block - old_base;
    FT_Byte**   offset = table->elements;
    FT_Byte**   limit  = offset + table->max_elems;


    for ( ; offset < limit; offset++ )
    {
      if ( offset[0] )
        offset[0] += delta;
    }
  }
