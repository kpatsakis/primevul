  Ins_CALL( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_ULong       F;
    TT_CallRec*    pCrec;
    TT_DefRecord*  def;


    /* first of all, check the index */

    F = (FT_ULong)args[0];
    if ( BOUNDSL( F, exc->maxFunc + 1 ) )
      goto Fail;

    /* Except for some old Apple fonts, all functions in a TrueType */
    /* font are defined in increasing order, starting from 0.  This */
    /* means that we normally have                                  */
    /*                                                              */
    /*    exc->maxFunc+1 == exc->numFDefs                           */
    /*    exc->FDefs[n].opc == n for n in 0..exc->maxFunc           */
    /*                                                              */
    /* If this isn't true, we need to look up the function table.   */

    def = exc->FDefs + F;
    if ( exc->maxFunc + 1 != exc->numFDefs || def->opc != F )
    {
      /* look up the FDefs table */
      TT_DefRecord*  limit;


      def   = exc->FDefs;
      limit = def + exc->numFDefs;

      while ( def < limit && def->opc != F )
        def++;

      if ( def == limit )
        goto Fail;
    }

    /* check that the function is active */
    if ( !def->active )
      goto Fail;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY                                    &&
         exc->ignore_x_mode                                             &&
         ( ( exc->iup_called                                        &&
             ( exc->sph_tweak_flags & SPH_TWEAK_NO_CALL_AFTER_IUP ) ) ||
           ( def->sph_fdef_flags & SPH_FDEF_VACUFORM_ROUND_1 )        ) )
      goto Fail;
    else
      exc->sph_in_func_flags = def->sph_fdef_flags;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    /* check the call stack */
    if ( exc->callTop >= exc->callSize )
    {
      exc->error = FT_THROW( Stack_Overflow );
      return;
    }

    pCrec = exc->callStack + exc->callTop;

    pCrec->Caller_Range = exc->curRange;
    pCrec->Caller_IP    = exc->IP + 1;
    pCrec->Cur_Count    = 1;
    pCrec->Def          = def;

    exc->callTop++;

    Ins_Goto_CodeRange( exc, def->range, def->start );

    exc->step_ins = FALSE;

    return;

  Fail:
    exc->error = FT_THROW( Invalid_Reference );
  }
