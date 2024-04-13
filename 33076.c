  Ins_SxVTL( TT_ExecContext  exc,
             FT_UShort       aIdx1,
             FT_UShort       aIdx2,
             FT_UnitVector*  Vec )
  {
    FT_Long     A, B, C;
    FT_Vector*  p1;
    FT_Vector*  p2;

    FT_Byte  opcode = exc->opcode;


    if ( BOUNDS( aIdx1, exc->zp2.n_points ) ||
         BOUNDS( aIdx2, exc->zp1.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return FAILURE;
    }

    p1 = exc->zp1.cur + aIdx2;
    p2 = exc->zp2.cur + aIdx1;

    A = SUB_LONG( p1->x, p2->x );
    B = SUB_LONG( p1->y, p2->y );

    /* If p1 == p2, SPvTL and SFvTL behave the same as */
    /* SPvTCA[X] and SFvTCA[X], respectively.          */
    /*                                                 */
    /* Confirmed by Greg Hitchcock.                    */

    if ( A == 0 && B == 0 )
    {
      A      = 0x4000;
      opcode = 0;
    }

    if ( ( opcode & 1 ) != 0 )
    {
      C = B;   /* counter clockwise rotation */
      B = A;
      A = NEG_LONG( C );
    }

    Normalize( A, B, Vec );

    return SUCCESS;
  }
