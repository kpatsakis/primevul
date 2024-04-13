  Normalize( EXEC_OP_ FT_F26Dot6      Vx,
                      FT_F26Dot6      Vy,
                      FT_UnitVector*  R )
  {
    FT_F26Dot6  W;
    FT_Bool     S1, S2;

    FT_UNUSED_EXEC;


    if ( FT_ABS( Vx ) < 0x10000L && FT_ABS( Vy ) < 0x10000L )
    {
      Vx *= 0x100;
      Vy *= 0x100;

      W = TT_VecLen( Vx, Vy );

      if ( W == 0 )
      {
        /* XXX: UNDOCUMENTED! It seems that it is possible to try   */
        /*      to normalize the vector (0,0).  Return immediately. */
        return SUCCESS;
      }

      R->x = (FT_F2Dot14)FT_MulDiv( Vx, 0x4000L, W );
      R->y = (FT_F2Dot14)FT_MulDiv( Vy, 0x4000L, W );

      return SUCCESS;
    }

    W = TT_VecLen( Vx, Vy );

    Vx = FT_MulDiv( Vx, 0x4000L, W );
    Vy = FT_MulDiv( Vy, 0x4000L, W );

    W = Vx * Vx + Vy * Vy;

    /* Now, we want that Sqrt( W ) = 0x4000 */
    /* Or 0x10000000 <= W < 0x10004000        */

    if ( Vx < 0 )
    {
      Vx = -Vx;
      S1 = TRUE;
    }
    else
      S1 = FALSE;

    if ( Vy < 0 )
    {
      Vy = -Vy;
      S2 = TRUE;
    }
    else
      S2 = FALSE;

    while ( W < 0x10000000L )
    {
      /* We need to increase W by a minimal amount */
      if ( Vx < Vy )
        Vx++;
      else
        Vy++;

      W = Vx * Vx + Vy * Vy;
    }

    while ( W >= 0x10004000L )
    {
      /* We need to decrease W by a minimal amount */
      if ( Vx < Vy )
        Vx--;
      else
        Vy--;

      W = Vx * Vx + Vy * Vy;
    }

    /* Note that in various cases, we can only  */
    /* compute a Sqrt(W) of 0x3FFF, eg. Vx = Vy */

    if ( S1 )
      Vx = -Vx;

    if ( S2 )
      Vy = -Vy;

    R->x = (FT_F2Dot14)Vx;   /* Type conversion */
    R->y = (FT_F2Dot14)Vy;   /* Type conversion */

    return SUCCESS;
  }
