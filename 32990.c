  Ins_ENDF( TT_ExecContext  exc )
  {
    TT_CallRec*  pRec;


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    exc->sph_in_func_flags = 0x0000;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    if ( exc->callTop <= 0 )     /* We encountered an ENDF without a call */
    {
      exc->error = FT_THROW( ENDF_In_Exec_Stream );
      return;
    }

    exc->callTop--;

    pRec = &exc->callStack[exc->callTop];

    pRec->Cur_Count--;

    exc->step_ins = FALSE;

    if ( pRec->Cur_Count > 0 )
    {
      exc->callTop++;
      exc->IP = pRec->Def->start;
    }
    else
      /* Loop through the current function */
      Ins_Goto_CodeRange( exc, pRec->Caller_Range, pRec->Caller_IP );

    /* Exit the current call frame.                      */

    /* NOTE: If the last instruction of a program is a   */
    /*       CALL or LOOPCALL, the return address is     */
    /*       always out of the code range.  This is a    */
    /*       valid address, and it is why we do not test */
    /*       the result of Ins_Goto_CodeRange() here!    */
  }
