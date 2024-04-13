  tt_face_get_kerning( TT_Face  face,
                       FT_UInt  left_glyph,
                       FT_UInt  right_glyph )
  {
    FT_Int    result = 0;
    FT_UInt   count, mask;
    FT_Byte*  p       = face->kern_table;
    FT_Byte*  p_limit = p + face->kern_table_size;


    p   += 4;
    mask = 0x0001;

    for ( count = face->num_kern_tables;
          count > 0 && p + 6 <= p_limit;
          count--, mask <<= 1 )
    {
      FT_Byte* base     = p;
      FT_Byte* next;
      FT_UInt  version  = FT_NEXT_USHORT( p );
      FT_UInt  length   = FT_NEXT_USHORT( p );
      FT_UInt  coverage = FT_NEXT_USHORT( p );
      FT_UInt  num_pairs;
      FT_Int   value    = 0;

      FT_UNUSED( version );


      next = base + length;

      if ( next > p_limit )  /* handle broken table */
        next = p_limit;

      if ( ( face->kern_avail_bits & mask ) == 0 )
        goto NextTable;

      if ( p + 8 > next )
        goto NextTable;

      num_pairs = FT_NEXT_USHORT( p );
      p        += 6;

      if ( ( next - p ) < 6 * (int)num_pairs )  /* handle broken count  */
        num_pairs = (FT_UInt)( ( next - p ) / 6 );

      switch ( coverage >> 8 )
      {
      case 0:
        {
          FT_ULong  key0 = TT_KERN_INDEX( left_glyph, right_glyph );


          if ( face->kern_order_bits & mask )   /* binary search */
          {
            FT_UInt   min = 0;
            FT_UInt   max = num_pairs;


            while ( min < max )
            {
              FT_UInt   mid = ( min + max ) >> 1;
              FT_Byte*  q   = p + 6 * mid;
              FT_ULong  key;


              key = FT_NEXT_ULONG( q );

              if ( key == key0 )
              {
                value = FT_PEEK_SHORT( q );
                goto Found;
              }
              if ( key < key0 )
                min = mid + 1;
              else
                max = mid;
            }
          }
          else /* linear search */
          {
            FT_UInt  count2;


            for ( count2 = num_pairs; count2 > 0; count2-- )
            {
              FT_ULong  key = FT_NEXT_ULONG( p );


              if ( key == key0 )
              {
                value = FT_PEEK_SHORT( p );
                goto Found;
              }
              p += 2;
            }
          }
        }
        break;

       /*
        *  We don't support format 2 because we haven't seen a single font
        *  using it in real life...
        */

      default:
        ;
      }

      goto NextTable;

    Found:
      if ( coverage & 8 ) /* override or add */
        result = value;
      else
        result += value;

    NextTable:
      p = next;
    }

    return result;
  }
