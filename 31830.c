  tt_cmap14_variant_chars( TT_CMap    cmap,
                           FT_Memory  memory,
                           FT_UInt32  variantSelector )
  {
    FT_Byte    *p  = tt_cmap14_find_variant( cmap->data + 6,
                                             variantSelector );
    FT_Int      i;
    FT_ULong    defOff;
    FT_ULong    nondefOff;


    if ( !p )
      return NULL;

    defOff    = TT_NEXT_ULONG( p );
    nondefOff = TT_NEXT_ULONG( p );

    if ( defOff == 0 && nondefOff == 0 )
      return NULL;

    if ( defOff == 0 )
      return tt_cmap14_get_nondef_chars( cmap, cmap->data + nondefOff,
                                         memory );
    else if ( nondefOff == 0 )
      return tt_cmap14_get_def_chars( cmap, cmap->data + defOff,
                                      memory );
    else
    {
      /* Both a default and a non-default glyph set?  That's probably not */
      /* good font design, but the spec allows for it...                  */
      TT_CMap14  cmap14 = (TT_CMap14) cmap;
      FT_UInt32  numRanges;
      FT_UInt32  numMappings;
      FT_UInt32  duni;
      FT_UInt32  dcnt;
      FT_UInt32  nuni;
      FT_Byte*   dp;
      FT_UInt    di, ni, k;

      FT_UInt32  *ret;


      p  = cmap->data + nondefOff;
      dp = cmap->data + defOff;

      numMappings = (FT_UInt32)TT_NEXT_ULONG( p );
      dcnt        = tt_cmap14_def_char_count( dp );
      numRanges   = (FT_UInt32)TT_NEXT_ULONG( dp );

      if ( numMappings == 0 )
        return tt_cmap14_get_def_chars( cmap, cmap->data + defOff,
                                        memory );
      if ( dcnt == 0 )
        return tt_cmap14_get_nondef_chars( cmap, cmap->data + nondefOff,
                                           memory );

      if ( tt_cmap14_ensure( cmap14, ( dcnt + numMappings + 1 ), memory ) )
        return NULL;

      ret  = cmap14->results;
      duni = (FT_UInt32)TT_NEXT_UINT24( dp );
      dcnt = FT_NEXT_BYTE( dp );
      di   = 1;
      nuni = (FT_UInt32)TT_NEXT_UINT24( p );
      p   += 2;
      ni   = 1;
      i    = 0;

      for ( ;; )
      {
        if ( nuni > duni + dcnt )
        {
          for ( k = 0; k <= dcnt; ++k )
            ret[i++] = duni + k;

          ++di;

          if ( di > numRanges )
            break;

          duni = (FT_UInt32)TT_NEXT_UINT24( dp );
          dcnt = FT_NEXT_BYTE( dp );
        }
        else
        {
          if ( nuni < duni )
            ret[i++] = nuni;
          /* If it is within the default range then ignore it -- */
          /* that should not have happened                       */
          ++ni;
          if ( ni > numMappings )
            break;

          nuni = (FT_UInt32)TT_NEXT_UINT24( p );
          p += 2;
        }
      }

      if ( ni <= numMappings )
      {
        /* If we get here then we have run out of all default ranges.   */
        /* We have read one non-default mapping which we haven't stored */
        /* and there may be others that need to be read.                */
        ret[i++] = nuni;
        while ( ni < numMappings )
        {
          ret[i++] = (FT_UInt32)TT_NEXT_UINT24( p );
          p += 2;
          ++ni;
        }
      }
      else if ( di <= numRanges )
      {
        /* If we get here then we have run out of all non-default     */
        /* mappings.  We have read one default range which we haven't */
        /* stored and there may be others that need to be read.       */
        for ( k = 0; k <= dcnt; ++k )
          ret[i++] = duni + k;

        while ( di < numRanges )
        {
          duni = (FT_UInt32)TT_NEXT_UINT24( dp );
          dcnt = FT_NEXT_BYTE( dp );

          for ( k = 0; k <= dcnt; ++k )
            ret[i++] = duni + k;
          ++di;
        }
      }

      ret[i] = 0;

      return ret;
    }
  }
