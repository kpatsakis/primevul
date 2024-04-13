  Project( TT_ExecContext  exc,
           FT_Pos          dx,
           FT_Pos          dy )
  {
    return TT_DotFix14( dx, dy,
                        exc->GS.projVector.x,
                        exc->GS.projVector.y );
  }
