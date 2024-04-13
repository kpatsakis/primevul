  TT_RunIns( TT_ExecContext  exc )
  {
    FT_ULong   ins_counter = 0;  /* executed instructions counter */
    FT_ULong   num_twilight_points;
    FT_UShort  i;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    FT_Byte    opcode_pattern[1][2] = {
                  /* #8 TypeMan Talk Align */
                  {
                    0x06, /* SPVTL   */
                    0x7D, /* RDTG    */
                  },
                };
    FT_UShort  opcode_patterns   = 1;
    FT_UShort  opcode_pointer[1] = { 0 };
    FT_UShort  opcode_size[1]    = { 1 };
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    exc->iup_called = FALSE;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    /*
     *  Toggle backward compatibility according to what font wants, except
     *  when
     *
     *  1) we have a `tricky' font that heavily relies on the interpreter to
     *     render glyphs correctly, for example DFKai-SB, or
     *  2) FT_RENDER_MODE_MONO (i.e, monochome rendering) is requested.
     *
     *  In those cases, backward compatibility needs to be turned off to get
     *  correct rendering.  The rendering is then completely up to the
     *  font's programming.
     *
     */
    if ( SUBPIXEL_HINTING_MINIMAL          &&
         exc->subpixel_hinting_lean        &&
         !FT_IS_TRICKY( &exc->face->root ) )
      exc->backward_compatibility = !( exc->GS.instruct_control & 4 );
    else
      exc->backward_compatibility = FALSE;

    exc->iupx_called = FALSE;
    exc->iupy_called = FALSE;
#endif

    /* We restrict the number of twilight points to a reasonable,     */
    /* heuristic value to avoid slow execution of malformed bytecode. */
    num_twilight_points = FT_MAX( 30,
                                  2 * ( exc->pts.n_points + exc->cvtSize ) );
    if ( exc->twilight.n_points > num_twilight_points )
    {
      if ( num_twilight_points > 0xFFFFU )
        num_twilight_points = 0xFFFFU;

      FT_TRACE5(( "TT_RunIns: Resetting number of twilight points\n"
                  "           from %d to the more reasonable value %d\n",
                  exc->twilight.n_points,
                  num_twilight_points ));
      exc->twilight.n_points = (FT_UShort)num_twilight_points;
    }

    /* Set up loop detectors.  We restrict the number of LOOPCALL loops */
    /* and the number of JMPR, JROT, and JROF calls with a negative     */
    /* argument to values that depend on various parameters like the    */
    /* size of the CVT table or the number of points in the current     */
    /* glyph (if applicable).                                           */
    /*                                                                  */
    /* The idea is that in real-world bytecode you either iterate over  */
    /* all CVT entries (in the `prep' table), or over all points (or    */
    /* contours, in the `glyf' table) of a glyph, and such iterations   */
    /* don't happen very often.                                         */
    exc->loopcall_counter = 0;
    exc->neg_jump_counter = 0;

    /* The maximum values are heuristic. */
    if ( exc->pts.n_points )
      exc->loopcall_counter_max = FT_MAX( 50,
                                          10 * exc->pts.n_points ) +
                                  FT_MAX( 50,
                                          exc->cvtSize / 10 );
    else
      exc->loopcall_counter_max = 300 + 8 * exc->cvtSize;

    /* as a protection against an unreasonable number of CVT entries  */
    /* we assume at most 100 control values per glyph for the counter */
    if ( exc->loopcall_counter_max >
         100 * (FT_ULong)exc->face->root.num_glyphs )
      exc->loopcall_counter_max = 100 * (FT_ULong)exc->face->root.num_glyphs;

    FT_TRACE5(( "TT_RunIns: Limiting total number of loops in LOOPCALL"
                " to %d\n", exc->loopcall_counter_max ));

    exc->neg_jump_counter_max = exc->loopcall_counter_max;
    FT_TRACE5(( "TT_RunIns: Limiting total number of backward jumps"
                " to %d\n", exc->neg_jump_counter_max ));

    /* set PPEM and CVT functions */
    exc->tt_metrics.ratio = 0;
    if ( exc->metrics.x_ppem != exc->metrics.y_ppem )
    {
      /* non-square pixels, use the stretched routines */
      exc->func_cur_ppem  = Current_Ppem_Stretched;
      exc->func_read_cvt  = Read_CVT_Stretched;
      exc->func_write_cvt = Write_CVT_Stretched;
      exc->func_move_cvt  = Move_CVT_Stretched;
    }
    else
    {
      /* square pixels, use normal routines */
      exc->func_cur_ppem  = Current_Ppem;
      exc->func_read_cvt  = Read_CVT;
      exc->func_write_cvt = Write_CVT;
      exc->func_move_cvt  = Move_CVT;
    }

    Compute_Funcs( exc );
    Compute_Round( exc, (FT_Byte)exc->GS.round_state );

    do
    {
      exc->opcode = exc->code[exc->IP];

#ifdef FT_DEBUG_LEVEL_TRACE
      {
        FT_Long  cnt = FT_MIN( 8, exc->top );
        FT_Long  n;


        /* if tracing level is 7, show current code position */
        /* and the first few stack elements also             */
        FT_TRACE6(( "  " ));
        FT_TRACE7(( "%06d ", exc->IP ));
        FT_TRACE6(( opcode_name[exc->opcode] + 2 ));
        FT_TRACE7(( "%*s", *opcode_name[exc->opcode] == 'A'
                              ? 2
                              : 12 - ( *opcode_name[exc->opcode] - '0' ),
                              "#" ));
        for ( n = 1; n <= cnt; n++ )
          FT_TRACE7(( " %d", exc->stack[exc->top - n] ));
        FT_TRACE6(( "\n" ));
      }
#endif /* FT_DEBUG_LEVEL_TRACE */

      if ( ( exc->length = opcode_length[exc->opcode] ) < 0 )
      {
        if ( exc->IP + 1 >= exc->codeSize )
          goto LErrorCodeOverflow_;

        exc->length = 2 - exc->length * exc->code[exc->IP + 1];
      }

      if ( exc->IP + exc->length > exc->codeSize )
        goto LErrorCodeOverflow_;

      /* First, let's check for empty stack and overflow */
      exc->args = exc->top - ( Pop_Push_Count[exc->opcode] >> 4 );

      /* `args' is the top of the stack once arguments have been popped. */
      /* One can also interpret it as the index of the last argument.    */
      if ( exc->args < 0 )
      {
        if ( exc->pedantic_hinting )
        {
          exc->error = FT_THROW( Too_Few_Arguments );
          goto LErrorLabel_;
        }

        /* push zeroes onto the stack */
        for ( i = 0; i < Pop_Push_Count[exc->opcode] >> 4; i++ )
          exc->stack[i] = 0;
        exc->args = 0;
      }

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
      if ( exc->opcode == 0x91 )
      {
        /* this is very special: GETVARIATION returns */
        /* a variable number of arguments             */

        /* it is the job of the application to `activate' GX handling, */
        /* this is, calling any of the GX API functions on the current */
        /* font to select a variation instance                         */
        if ( exc->face->blend )
          exc->new_top = exc->args + exc->face->blend->num_axis;
      }
      else
#endif
        exc->new_top = exc->args + ( Pop_Push_Count[exc->opcode] & 15 );

      /* `new_top' is the new top of the stack, after the instruction's */
      /* execution.  `top' will be set to `new_top' after the `switch'  */
      /* statement.                                                     */
      if ( exc->new_top > exc->stackSize )
      {
        exc->error = FT_THROW( Stack_Overflow );
        goto LErrorLabel_;
      }

      exc->step_ins = TRUE;
      exc->error    = FT_Err_Ok;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY

      if ( SUBPIXEL_HINTING_INFINALITY )
      {
        for ( i = 0; i < opcode_patterns; i++ )
        {
          if ( opcode_pointer[i] < opcode_size[i]                  &&
               exc->opcode == opcode_pattern[i][opcode_pointer[i]] )
          {
            opcode_pointer[i] += 1;

            if ( opcode_pointer[i] == opcode_size[i] )
            {
              FT_TRACE6(( "sph: opcode ptrn: %d, %s %s\n",
                          i,
                          exc->face->root.family_name,
                          exc->face->root.style_name ));

              switch ( i )
              {
              case 0:
                break;
              }
              opcode_pointer[i] = 0;
            }
          }
          else
            opcode_pointer[i] = 0;
        }
      }

#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

      {
        FT_Long*  args   = exc->stack + exc->args;
        FT_Byte   opcode = exc->opcode;


        switch ( opcode )
        {
        case 0x00:  /* SVTCA y  */
        case 0x01:  /* SVTCA x  */
        case 0x02:  /* SPvTCA y */
        case 0x03:  /* SPvTCA x */
        case 0x04:  /* SFvTCA y */
        case 0x05:  /* SFvTCA x */
          Ins_SxyTCA( exc );
          break;

        case 0x06:  /* SPvTL // */
        case 0x07:  /* SPvTL +  */
          Ins_SPVTL( exc, args );
          break;

        case 0x08:  /* SFvTL // */
        case 0x09:  /* SFvTL +  */
          Ins_SFVTL( exc, args );
          break;

        case 0x0A:  /* SPvFS */
          Ins_SPVFS( exc, args );
          break;

        case 0x0B:  /* SFvFS */
          Ins_SFVFS( exc, args );
          break;

        case 0x0C:  /* GPv */
          Ins_GPV( exc, args );
          break;

        case 0x0D:  /* GFv */
          Ins_GFV( exc, args );
          break;

        case 0x0E:  /* SFvTPv */
          Ins_SFVTPV( exc );
          break;

        case 0x0F:  /* ISECT  */
          Ins_ISECT( exc, args );
          break;

        case 0x10:  /* SRP0 */
          Ins_SRP0( exc, args );
          break;

        case 0x11:  /* SRP1 */
          Ins_SRP1( exc, args );
          break;

        case 0x12:  /* SRP2 */
          Ins_SRP2( exc, args );
          break;

        case 0x13:  /* SZP0 */
          Ins_SZP0( exc, args );
          break;

        case 0x14:  /* SZP1 */
          Ins_SZP1( exc, args );
          break;

        case 0x15:  /* SZP2 */
          Ins_SZP2( exc, args );
          break;

        case 0x16:  /* SZPS */
          Ins_SZPS( exc, args );
          break;

        case 0x17:  /* SLOOP */
          Ins_SLOOP( exc, args );
          break;

        case 0x18:  /* RTG */
          Ins_RTG( exc );
          break;

        case 0x19:  /* RTHG */
          Ins_RTHG( exc );
          break;

        case 0x1A:  /* SMD */
          Ins_SMD( exc, args );
          break;

        case 0x1B:  /* ELSE */
          Ins_ELSE( exc );
          break;

        case 0x1C:  /* JMPR */
          Ins_JMPR( exc, args );
          break;

        case 0x1D:  /* SCVTCI */
          Ins_SCVTCI( exc, args );
          break;

        case 0x1E:  /* SSWCI */
          Ins_SSWCI( exc, args );
          break;

        case 0x1F:  /* SSW */
          Ins_SSW( exc, args );
          break;

        case 0x20:  /* DUP */
          Ins_DUP( args );
          break;

        case 0x21:  /* POP */
          Ins_POP();
          break;

        case 0x22:  /* CLEAR */
          Ins_CLEAR( exc );
          break;

        case 0x23:  /* SWAP */
          Ins_SWAP( args );
          break;

        case 0x24:  /* DEPTH */
          Ins_DEPTH( exc, args );
          break;

        case 0x25:  /* CINDEX */
          Ins_CINDEX( exc, args );
          break;

        case 0x26:  /* MINDEX */
          Ins_MINDEX( exc, args );
          break;

        case 0x27:  /* ALIGNPTS */
          Ins_ALIGNPTS( exc, args );
          break;

        case 0x28:  /* RAW */
          Ins_UNKNOWN( exc );
          break;

        case 0x29:  /* UTP */
          Ins_UTP( exc, args );
          break;

        case 0x2A:  /* LOOPCALL */
          Ins_LOOPCALL( exc, args );
          break;

        case 0x2B:  /* CALL */
          Ins_CALL( exc, args );
          break;

        case 0x2C:  /* FDEF */
          Ins_FDEF( exc, args );
          break;

        case 0x2D:  /* ENDF */
          Ins_ENDF( exc );
          break;

        case 0x2E:  /* MDAP */
        case 0x2F:  /* MDAP */
          Ins_MDAP( exc, args );
          break;

        case 0x30:  /* IUP */
        case 0x31:  /* IUP */
          Ins_IUP( exc );
          break;

        case 0x32:  /* SHP */
        case 0x33:  /* SHP */
          Ins_SHP( exc );
          break;

        case 0x34:  /* SHC */
        case 0x35:  /* SHC */
          Ins_SHC( exc, args );
          break;

        case 0x36:  /* SHZ */
        case 0x37:  /* SHZ */
          Ins_SHZ( exc, args );
          break;

        case 0x38:  /* SHPIX */
          Ins_SHPIX( exc, args );
          break;

        case 0x39:  /* IP    */
          Ins_IP( exc );
          break;

        case 0x3A:  /* MSIRP */
        case 0x3B:  /* MSIRP */
          Ins_MSIRP( exc, args );
          break;

        case 0x3C:  /* AlignRP */
          Ins_ALIGNRP( exc );
          break;

        case 0x3D:  /* RTDG */
          Ins_RTDG( exc );
          break;

        case 0x3E:  /* MIAP */
        case 0x3F:  /* MIAP */
          Ins_MIAP( exc, args );
          break;

        case 0x40:  /* NPUSHB */
          Ins_NPUSHB( exc, args );
          break;

        case 0x41:  /* NPUSHW */
          Ins_NPUSHW( exc, args );
          break;

        case 0x42:  /* WS */
          Ins_WS( exc, args );
          break;

        case 0x43:  /* RS */
          Ins_RS( exc, args );
          break;

        case 0x44:  /* WCVTP */
          Ins_WCVTP( exc, args );
          break;

        case 0x45:  /* RCVT */
          Ins_RCVT( exc, args );
          break;

        case 0x46:  /* GC */
        case 0x47:  /* GC */
          Ins_GC( exc, args );
          break;

        case 0x48:  /* SCFS */
          Ins_SCFS( exc, args );
          break;

        case 0x49:  /* MD */
        case 0x4A:  /* MD */
          Ins_MD( exc, args );
          break;

        case 0x4B:  /* MPPEM */
          Ins_MPPEM( exc, args );
          break;

        case 0x4C:  /* MPS */
          Ins_MPS( exc, args );
          break;

        case 0x4D:  /* FLIPON */
          Ins_FLIPON( exc );
          break;

        case 0x4E:  /* FLIPOFF */
          Ins_FLIPOFF( exc );
          break;

        case 0x4F:  /* DEBUG */
          Ins_DEBUG( exc );
          break;

        case 0x50:  /* LT */
          Ins_LT( args );
          break;

        case 0x51:  /* LTEQ */
          Ins_LTEQ( args );
          break;

        case 0x52:  /* GT */
          Ins_GT( args );
          break;

        case 0x53:  /* GTEQ */
          Ins_GTEQ( args );
          break;

        case 0x54:  /* EQ */
          Ins_EQ( args );
          break;

        case 0x55:  /* NEQ */
          Ins_NEQ( args );
          break;

        case 0x56:  /* ODD */
          Ins_ODD( exc, args );
          break;

        case 0x57:  /* EVEN */
          Ins_EVEN( exc, args );
          break;

        case 0x58:  /* IF */
          Ins_IF( exc, args );
          break;

        case 0x59:  /* EIF */
          Ins_EIF();
          break;

        case 0x5A:  /* AND */
          Ins_AND( args );
          break;

        case 0x5B:  /* OR */
          Ins_OR( args );
          break;

        case 0x5C:  /* NOT */
          Ins_NOT( args );
          break;

        case 0x5D:  /* DELTAP1 */
          Ins_DELTAP( exc, args );
          break;

        case 0x5E:  /* SDB */
          Ins_SDB( exc, args );
          break;

        case 0x5F:  /* SDS */
          Ins_SDS( exc, args );
          break;

        case 0x60:  /* ADD */
          Ins_ADD( args );
          break;

        case 0x61:  /* SUB */
          Ins_SUB( args );
          break;

        case 0x62:  /* DIV */
          Ins_DIV( exc, args );
          break;

        case 0x63:  /* MUL */
          Ins_MUL( args );
          break;

        case 0x64:  /* ABS */
          Ins_ABS( args );
          break;

        case 0x65:  /* NEG */
          Ins_NEG( args );
          break;

        case 0x66:  /* FLOOR */
          Ins_FLOOR( args );
          break;

        case 0x67:  /* CEILING */
          Ins_CEILING( args );
          break;

        case 0x68:  /* ROUND */
        case 0x69:  /* ROUND */
        case 0x6A:  /* ROUND */
        case 0x6B:  /* ROUND */
          Ins_ROUND( exc, args );
          break;

        case 0x6C:  /* NROUND */
        case 0x6D:  /* NROUND */
        case 0x6E:  /* NRRUND */
        case 0x6F:  /* NROUND */
          Ins_NROUND( exc, args );
          break;

        case 0x70:  /* WCVTF */
          Ins_WCVTF( exc, args );
          break;

        case 0x71:  /* DELTAP2 */
        case 0x72:  /* DELTAP3 */
          Ins_DELTAP( exc, args );
          break;

        case 0x73:  /* DELTAC0 */
        case 0x74:  /* DELTAC1 */
        case 0x75:  /* DELTAC2 */
          Ins_DELTAC( exc, args );
          break;

        case 0x76:  /* SROUND */
          Ins_SROUND( exc, args );
          break;

        case 0x77:  /* S45Round */
          Ins_S45ROUND( exc, args );
          break;

        case 0x78:  /* JROT */
          Ins_JROT( exc, args );
          break;

        case 0x79:  /* JROF */
          Ins_JROF( exc, args );
          break;

        case 0x7A:  /* ROFF */
          Ins_ROFF( exc );
          break;

        case 0x7B:  /* ???? */
          Ins_UNKNOWN( exc );
          break;

        case 0x7C:  /* RUTG */
          Ins_RUTG( exc );
          break;

        case 0x7D:  /* RDTG */
          Ins_RDTG( exc );
          break;

        case 0x7E:  /* SANGW */
          Ins_SANGW();
          break;

        case 0x7F:  /* AA */
          Ins_AA();
          break;

        case 0x80:  /* FLIPPT */
          Ins_FLIPPT( exc );
          break;

        case 0x81:  /* FLIPRGON */
          Ins_FLIPRGON( exc, args );
          break;

        case 0x82:  /* FLIPRGOFF */
          Ins_FLIPRGOFF( exc, args );
          break;

        case 0x83:  /* UNKNOWN */
        case 0x84:  /* UNKNOWN */
          Ins_UNKNOWN( exc );
          break;

        case 0x85:  /* SCANCTRL */
          Ins_SCANCTRL( exc, args );
          break;

        case 0x86:  /* SDPvTL */
        case 0x87:  /* SDPvTL */
          Ins_SDPVTL( exc, args );
          break;

        case 0x88:  /* GETINFO */
          Ins_GETINFO( exc, args );
          break;

        case 0x89:  /* IDEF */
          Ins_IDEF( exc, args );
          break;

        case 0x8A:  /* ROLL */
          Ins_ROLL( args );
          break;

        case 0x8B:  /* MAX */
          Ins_MAX( args );
          break;

        case 0x8C:  /* MIN */
          Ins_MIN( args );
          break;

        case 0x8D:  /* SCANTYPE */
          Ins_SCANTYPE( exc, args );
          break;

        case 0x8E:  /* INSTCTRL */
          Ins_INSTCTRL( exc, args );
          break;

        case 0x8F:  /* ADJUST */
        case 0x90:  /* ADJUST */
          Ins_UNKNOWN( exc );
          break;

#ifdef TT_CONFIG_OPTION_GX_VAR_SUPPORT
        case 0x91:
          /* it is the job of the application to `activate' GX handling, */
          /* this is, calling any of the GX API functions on the current */
          /* font to select a variation instance                         */
          if ( exc->face->blend )
            Ins_GETVARIATION( exc, args );
          else
            Ins_UNKNOWN( exc );
          break;

        case 0x92:
          /* there is at least one MS font (LaoUI.ttf version 5.01) that */
          /* uses IDEFs for 0x91 and 0x92; for this reason we activate   */
          /* GETDATA for GX fonts only, similar to GETVARIATION          */
          if ( exc->face->blend )
            Ins_GETDATA( args );
          else
            Ins_UNKNOWN( exc );
          break;
#endif

        default:
          if ( opcode >= 0xE0 )
            Ins_MIRP( exc, args );
          else if ( opcode >= 0xC0 )
            Ins_MDRP( exc, args );
          else if ( opcode >= 0xB8 )
            Ins_PUSHW( exc, args );
          else if ( opcode >= 0xB0 )
            Ins_PUSHB( exc, args );
          else
            Ins_UNKNOWN( exc );
        }
      }

      if ( exc->error )
      {
        switch ( exc->error )
        {
          /* looking for redefined instructions */
        case FT_ERR( Invalid_Opcode ):
          {
            TT_DefRecord*  def   = exc->IDefs;
            TT_DefRecord*  limit = def + exc->numIDefs;


            for ( ; def < limit; def++ )
            {
              if ( def->active && exc->opcode == (FT_Byte)def->opc )
              {
                TT_CallRec*  callrec;


                if ( exc->callTop >= exc->callSize )
                {
                  exc->error = FT_THROW( Invalid_Reference );
                  goto LErrorLabel_;
                }

                callrec = &exc->callStack[exc->callTop];

                callrec->Caller_Range = exc->curRange;
                callrec->Caller_IP    = exc->IP + 1;
                callrec->Cur_Count    = 1;
                callrec->Def          = def;

                if ( Ins_Goto_CodeRange( exc,
                                         def->range,
                                         def->start ) == FAILURE )
                  goto LErrorLabel_;

                goto LSuiteLabel_;
              }
            }
          }

          exc->error = FT_THROW( Invalid_Opcode );
          goto LErrorLabel_;

#if 0
          break;   /* Unreachable code warning suppression.             */
                   /* Leave to remind in case a later change the editor */
                   /* to consider break;                                */
#endif

        default:
          goto LErrorLabel_;

#if 0
        break;
#endif
        }
      }

      exc->top = exc->new_top;

      if ( exc->step_ins )
        exc->IP += exc->length;

      /* increment instruction counter and check if we didn't */
      /* run this program for too long (e.g. infinite loops). */
      if ( ++ins_counter > TT_CONFIG_OPTION_MAX_RUNNABLE_OPCODES )
        return FT_THROW( Execution_Too_Long );

    LSuiteLabel_:
      if ( exc->IP >= exc->codeSize )
      {
        if ( exc->callTop > 0 )
        {
          exc->error = FT_THROW( Code_Overflow );
          goto LErrorLabel_;
        }
        else
          goto LNo_Error_;
      }
    } while ( !exc->instruction_trap );

  LNo_Error_:
    FT_TRACE4(( "  %d instruction%s executed\n",
                ins_counter == 1 ? "" : "s",
                ins_counter ));
    return FT_Err_Ok;

  LErrorCodeOverflow_:
    exc->error = FT_THROW( Code_Overflow );

  LErrorLabel_:
    if ( exc->error && !exc->instruction_trap )
      FT_TRACE1(( "  The interpreter returned error 0x%x\n", exc->error ));

    return exc->error;
  }
