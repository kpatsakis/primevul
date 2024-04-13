  Ins_INSTCTRL( TT_ExecContext  exc,
                FT_Long*        args )
  {
    FT_ULong  K, L, Kf;


    K = (FT_ULong)args[1];
    L = (FT_ULong)args[0];

    /* selector values cannot be `OR'ed;                 */
    /* they are indices starting with index 1, not flags */
    if ( K < 1 || K > 3 )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    /* convert index to flag value */
    Kf = 1 << ( K - 1 );

    if ( L != 0 )
    {
      /* arguments to selectors look like flag values */
      if ( L != Kf )
      {
        if ( exc->pedantic_hinting )
          exc->error = FT_THROW( Invalid_Reference );
        return;
      }
    }

    exc->GS.instruct_control &= ~(FT_Byte)Kf;
    exc->GS.instruct_control |= (FT_Byte)L;

    if ( K == 3 )
    {
#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
      /* INSTCTRL modifying flag 3 also has an effect */
      /* outside of the CVT program                   */
      if ( SUBPIXEL_HINTING_INFINALITY )
        exc->ignore_x_mode = FT_BOOL( L == 4 );
#endif

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
      /* Native ClearType fonts sign a waiver that turns off all backward  */
      /* compatibility hacks and lets them program points to the grid like */
      /* it's 1996.  They might sign a waiver for just one glyph, though.  */
      if ( SUBPIXEL_HINTING_MINIMAL )
        exc->backward_compatibility = !FT_BOOL( L == 4 );
#endif
    }
  }
