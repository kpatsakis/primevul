  Compute_Funcs( TT_ExecContext  exc )
  {
    if ( exc->GS.freeVector.x == 0x4000 )
      exc->F_dot_P = exc->GS.projVector.x;
    else if ( exc->GS.freeVector.y == 0x4000 )
      exc->F_dot_P = exc->GS.projVector.y;
    else
      exc->F_dot_P =
        ( (FT_Long)exc->GS.projVector.x * exc->GS.freeVector.x +
          (FT_Long)exc->GS.projVector.y * exc->GS.freeVector.y ) >> 14;

    if ( exc->GS.projVector.x == 0x4000 )
      exc->func_project = (TT_Project_Func)Project_x;
    else if ( exc->GS.projVector.y == 0x4000 )
      exc->func_project = (TT_Project_Func)Project_y;
    else
      exc->func_project = (TT_Project_Func)Project;

    if ( exc->GS.dualVector.x == 0x4000 )
      exc->func_dualproj = (TT_Project_Func)Project_x;
    else if ( exc->GS.dualVector.y == 0x4000 )
      exc->func_dualproj = (TT_Project_Func)Project_y;
    else
      exc->func_dualproj = (TT_Project_Func)Dual_Project;

    exc->func_move      = (TT_Move_Func)Direct_Move;
    exc->func_move_orig = (TT_Move_Func)Direct_Move_Orig;

    if ( exc->F_dot_P == 0x4000L )
    {
      if ( exc->GS.freeVector.x == 0x4000 )
      {
        exc->func_move      = (TT_Move_Func)Direct_Move_X;
        exc->func_move_orig = (TT_Move_Func)Direct_Move_Orig_X;
      }
      else if ( exc->GS.freeVector.y == 0x4000 )
      {
        exc->func_move      = (TT_Move_Func)Direct_Move_Y;
        exc->func_move_orig = (TT_Move_Func)Direct_Move_Orig_Y;
      }
    }

    /* at small sizes, F_dot_P can become too small, resulting   */
    /* in overflows and `spikes' in a number of glyphs like `w'. */

    if ( FT_ABS( exc->F_dot_P ) < 0x400L )
      exc->F_dot_P = 0x4000L;

    /* Disable cached aspect ratio */
    exc->tt_metrics.ratio = 0;
  }
