  tt_size_run_prep( TT_Size  size,
                    FT_Bool  pedantic )
  {
    TT_Face         face = (TT_Face)size->root.face;
    TT_ExecContext  exec;
    FT_Error        error;


    exec = size->context;

    error = TT_Load_Context( exec, face, size );
    if ( error )
      return error;

    exec->callTop = 0;
    exec->top     = 0;

    exec->instruction_trap = FALSE;

    exec->pedantic_hinting = pedantic;

    TT_Set_CodeRange( exec,
                      tt_coderange_cvt,
                      face->cvt_program,
                      (FT_Long)face->cvt_program_size );

    TT_Clear_CodeRange( exec, tt_coderange_glyph );

    if ( face->cvt_program_size > 0 )
    {
      TT_Goto_CodeRange( exec, tt_coderange_cvt, 0 );

      FT_TRACE4(( "Executing `prep' table.\n" ));
      error = face->interpreter( exec );
#ifdef FT_DEBUG_LEVEL_TRACE
      if ( error )
        FT_TRACE4(( "  interpretation failed with error code 0x%x\n",
                    error ));
#endif
    }
    else
      error = FT_Err_Ok;

    size->cvt_ready = error;

    /* UNDOCUMENTED!  The MS rasterizer doesn't allow the following */
    /* graphics state variables to be modified by the CVT program.  */

    exec->GS.dualVector.x = 0x4000;
    exec->GS.dualVector.y = 0;
    exec->GS.projVector.x = 0x4000;
    exec->GS.projVector.y = 0x0;
    exec->GS.freeVector.x = 0x4000;
    exec->GS.freeVector.y = 0x0;

    exec->GS.rp0 = 0;
    exec->GS.rp1 = 0;
    exec->GS.rp2 = 0;

    exec->GS.gep0 = 1;
    exec->GS.gep1 = 1;
    exec->GS.gep2 = 1;

    exec->GS.loop = 1;

    /* save as default graphics state */
    size->GS = exec->GS;

    TT_Save_Context( exec, size );

    return error;
  }
