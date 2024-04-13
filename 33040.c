  Ins_RS( TT_ExecContext  exc,
          FT_Long*        args )
  {
    FT_ULong  I = (FT_ULong)args[0];


    if ( BOUNDSL( I, exc->storeSize ) )
    {
      if ( exc->pedantic_hinting )
        ARRAY_BOUND_ERROR;
      else
        args[0] = 0;
    }
    else
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      /* subpixel hinting - avoid Typeman Dstroke and */
      /* IStroke and Vacuform rounds                  */
      if ( SUBPIXEL_HINTING_INFINALITY                 &&
           exc->ignore_x_mode                          &&
           ( ( I == 24                             &&
               ( exc->face->sph_found_func_flags &
                 ( SPH_FDEF_SPACING_1 |
                   SPH_FDEF_SPACING_2 )          ) ) ||
             ( I == 22                      &&
               ( exc->sph_in_func_flags   &
                 SPH_FDEF_TYPEMAN_STROKES ) )        ||
             ( I == 8                              &&
               ( exc->face->sph_found_func_flags &
                 SPH_FDEF_VACUFORM_ROUND_1       ) &&
               exc->iup_called                     ) ) )
        args[0] = 0;
      else
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */
        args[0] = exc->storage[I];
    }
  }
