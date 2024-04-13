  tt_cmap4_next( TT_CMap4  cmap )
  {
    FT_UInt  charcode;


    if ( cmap->cur_charcode >= 0xFFFFUL )
      goto Fail;

    charcode = (FT_UInt)cmap->cur_charcode + 1;

    if ( charcode < cmap->cur_start )
      charcode = cmap->cur_start;

    for ( ;; )
    {
      FT_Byte*  values = cmap->cur_values;
      FT_UInt   end    = cmap->cur_end;
      FT_Int    delta  = cmap->cur_delta;


      if ( charcode <= end )
      {
        if ( values )
        {
          FT_Byte*  p = values + 2 * ( charcode - cmap->cur_start );


          do
          {
            FT_UInt  gindex = FT_NEXT_USHORT( p );


            if ( gindex != 0 )
            {
              gindex = (FT_UInt)( ( gindex + delta ) & 0xFFFFU );
              if ( gindex != 0 )
              {
                cmap->cur_charcode = charcode;
                cmap->cur_gindex   = gindex;
                return;
              }
            }
          } while ( ++charcode <= end );
        }
        else
        {
          do
          {
            FT_UInt  gindex = (FT_UInt)( ( charcode + delta ) & 0xFFFFU );


            if ( gindex != 0 )
            {
              cmap->cur_charcode = charcode;
              cmap->cur_gindex   = gindex;
              return;
            }
          } while ( ++charcode <= end );
        }
      }

      /* we need to find another range */
      if ( tt_cmap4_set_range( cmap, cmap->cur_range + 1 ) < 0 )
        break;

      if ( charcode < cmap->cur_start )
        charcode = cmap->cur_start;
    }

  Fail:
    cmap->cur_charcode = (FT_UInt32)0xFFFFFFFFUL;
    cmap->cur_gindex   = 0;
  }
