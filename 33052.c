  Ins_SDPVTL( TT_ExecContext  exc,
              FT_Long*        args )
  {
    FT_Long    A, B, C;
    FT_UShort  p1, p2;            /* was FT_Int in pas type ERROR */

    FT_Byte  opcode = exc->opcode;


    p1 = (FT_UShort)args[1];
    p2 = (FT_UShort)args[0];

    if ( BOUNDS( p2, exc->zp1.n_points ) ||
         BOUNDS( p1, exc->zp2.n_points ) )
    {
      if ( exc->pedantic_hinting )
        exc->error = FT_THROW( Invalid_Reference );
      return;
    }

    {
      FT_Vector*  v1 = exc->zp1.org + p2;
      FT_Vector*  v2 = exc->zp2.org + p1;


      A = SUB_LONG( v1->x, v2->x );
      B = SUB_LONG( v1->y, v2->y );

      /* If v1 == v2, SDPvTL behaves the same as */
      /* SVTCA[X], respectively.                 */
      /*                                         */
      /* Confirmed by Greg Hitchcock.            */

      if ( A == 0 && B == 0 )
      {
        A      = 0x4000;
        opcode = 0;
      }
    }

    if ( ( opcode & 1 ) != 0 )
    {
      C = B;   /* counter clockwise rotation */
      B = A;
      A = NEG_LONG( C );
    }

    Normalize( A, B, &exc->GS.dualVector );

    {
      FT_Vector*  v1 = exc->zp1.cur + p2;
      FT_Vector*  v2 = exc->zp2.cur + p1;


      A = SUB_LONG( v1->x, v2->x );
      B = SUB_LONG( v1->y, v2->y );

      if ( A == 0 && B == 0 )
      {
        A      = 0x4000;
        opcode = 0;
      }
    }

    if ( ( opcode & 1 ) != 0 )
    {
      C = B;   /* counter clockwise rotation */
      B = A;
      A = NEG_LONG( C );
    }

    Normalize( A, B, &exc->GS.projVector );
    Compute_Funcs( exc );
  }
