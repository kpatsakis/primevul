  Compute_Funcs( EXEC_OP )
  {
#ifdef TT_CONFIG_OPTION_UNPATENTED_HINTING
    if ( CUR.face->unpatented_hinting )
    {
      /* If both vectors point rightwards along the x axis, set             */
      /* `both-x-axis' true, otherwise set it false.  The x values only     */
      /* need be tested because the vector has been normalised to a unit    */
      /* vector of length 0x4000 = unity.                                   */
      CUR.GS.both_x_axis = (FT_Bool)( CUR.GS.projVector.x == 0x4000 &&
                                      CUR.GS.freeVector.x == 0x4000 );

      /* Throw away projection and freedom vector information */
      /* because the patents don't allow them to be stored.   */
      /* The relevant US Patents are 5155805 and 5325479.     */
      CUR.GS.projVector.x = 0;
      CUR.GS.projVector.y = 0;
      CUR.GS.freeVector.x = 0;
      CUR.GS.freeVector.y = 0;

      if ( CUR.GS.both_x_axis )
      {
        CUR.func_project   = Project_x;
        CUR.func_move      = Direct_Move_X;
        CUR.func_move_orig = Direct_Move_Orig_X;
      }
      else
      {
        CUR.func_project   = Project_y;
        CUR.func_move      = Direct_Move_Y;
        CUR.func_move_orig = Direct_Move_Orig_Y;
      }

      if ( CUR.GS.dualVector.x == 0x4000 )
        CUR.func_dualproj = Project_x;
      else
      {
        if ( CUR.GS.dualVector.y == 0x4000 )
          CUR.func_dualproj = Project_y;
        else
          CUR.func_dualproj = Dual_Project;
      }

      /* Force recalculation of cached aspect ratio */
      CUR.tt_metrics.ratio = 0;

      return;
    }
#endif /* TT_CONFIG_OPTION_UNPATENTED_HINTING */

    if ( CUR.GS.freeVector.x == 0x4000 )
      CUR.F_dot_P       = CUR.GS.projVector.x * 0x10000L;
    else
    {
      if ( CUR.GS.freeVector.y == 0x4000 )
        CUR.F_dot_P       = CUR.GS.projVector.y * 0x10000L;
      else
        CUR.F_dot_P = (FT_Long)CUR.GS.projVector.x * CUR.GS.freeVector.x * 4 +
                      (FT_Long)CUR.GS.projVector.y * CUR.GS.freeVector.y * 4;
    }

    if ( CUR.GS.projVector.x == 0x4000 )
      CUR.func_project = (TT_Project_Func)Project_x;
    else
    {
      if ( CUR.GS.projVector.y == 0x4000 )
        CUR.func_project = (TT_Project_Func)Project_y;
      else
        CUR.func_project = (TT_Project_Func)Project;
    }

    if ( CUR.GS.dualVector.x == 0x4000 )
      CUR.func_dualproj = (TT_Project_Func)Project_x;
    else
    {
      if ( CUR.GS.dualVector.y == 0x4000 )
        CUR.func_dualproj = (TT_Project_Func)Project_y;
      else
        CUR.func_dualproj = (TT_Project_Func)Dual_Project;
    }

    CUR.func_move      = (TT_Move_Func)Direct_Move;
    CUR.func_move_orig = (TT_Move_Func)Direct_Move_Orig;

    if ( CUR.F_dot_P == 0x40000000L )
    {
      if ( CUR.GS.freeVector.x == 0x4000 )
      {
        CUR.func_move      = (TT_Move_Func)Direct_Move_X;
        CUR.func_move_orig = (TT_Move_Func)Direct_Move_Orig_X;
      }
      else
      {
        if ( CUR.GS.freeVector.y == 0x4000 )
        {
          CUR.func_move      = (TT_Move_Func)Direct_Move_Y;
          CUR.func_move_orig = (TT_Move_Func)Direct_Move_Orig_Y;
        }
      }
    }

    /* at small sizes, F_dot_P can become too small, resulting   */
    /* in overflows and `spikes' in a number of glyphs like `w'. */

    if ( FT_ABS( CUR.F_dot_P ) < 0x4000000L )
      CUR.F_dot_P = 0x40000000L;

    /* Disable cached aspect ratio */
    CUR.tt_metrics.ratio = 0;
  }
