  Dual_Project( EXEC_OP_ FT_Pos  dx,
                         FT_Pos  dy )
  {
    return TT_DotFix14( (FT_UInt32)dx, (FT_UInt32)dy,
                        CUR.GS.dualVector.x,
                        CUR.GS.dualVector.y );
  }
