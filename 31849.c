  ps_table_add( PS_Table  table,
                FT_Int    idx,
                void*     object,
                FT_UInt   length )
  {
    if ( idx < 0 || idx >= table->max_elems )
    {
      FT_ERROR(( "ps_table_add: invalid index\n" ));
      return FT_THROW( Invalid_Argument );
    }

    /* grow the base block if needed */
    if ( table->cursor + length > table->capacity )
    {
      FT_Error    error;
      FT_Offset   new_size = table->capacity;
      FT_PtrDist  in_offset;


      in_offset = (FT_Byte*)object - table->block;
      if ( in_offset < 0 || (FT_Offset)in_offset >= table->capacity )
        in_offset = -1;

      while ( new_size < table->cursor + length )
      {
        /* increase size by 25% and round up to the nearest multiple
           of 1024 */
        new_size += ( new_size >> 2 ) + 1;
        new_size  = FT_PAD_CEIL( new_size, 1024 );
      }

      error = reallocate_t1_table( table, new_size );
      if ( error )
        return error;

      if ( in_offset >= 0 )
        object = table->block + in_offset;
    }

    /* add the object to the base block and adjust offset */
    table->elements[idx] = table->block + table->cursor;
    table->lengths [idx] = length;
    FT_MEM_COPY( table->block + table->cursor, object, length );

    table->cursor += length;
    return FT_Err_Ok;
  }
