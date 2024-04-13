  Ins_FDEF( TT_ExecContext  exc,
            FT_Long*        args )
  {
    FT_ULong       n;
    TT_DefRecord*  rec;
    TT_DefRecord*  limit;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    /* arguments to opcodes are skipped by `SKIP_Code' */
    FT_Byte    opcode_pattern[9][12] = {
                 /* #0 inline delta function 1 */
                 {
                   0x4B, /* PPEM    */
                   0x53, /* GTEQ    */
                   0x23, /* SWAP    */
                   0x4B, /* PPEM    */
                   0x51, /* LTEQ    */
                   0x5A, /* AND     */
                   0x58, /* IF      */
                   0x38, /*   SHPIX */
                   0x1B, /* ELSE    */
                   0x21, /*   POP   */
                   0x21, /*   POP   */
                   0x59  /* EIF     */
                 },
                 /* #1 inline delta function 2 */
                 {
                   0x4B, /* PPEM    */
                   0x54, /* EQ      */
                   0x58, /* IF      */
                   0x38, /*   SHPIX */
                   0x1B, /* ELSE    */
                   0x21, /*   POP   */
                   0x21, /*   POP   */
                   0x59  /* EIF     */
                 },
                 /* #2 diagonal stroke function */
                 {
                   0x20, /* DUP     */
                   0x20, /* DUP     */
                   0xB0, /* PUSHB_1 */
                         /*   1     */
                   0x60, /* ADD     */
                   0x46, /* GC_cur  */
                   0xB0, /* PUSHB_1 */
                         /*   64    */
                   0x23, /* SWAP    */
                   0x42  /* WS      */
                 },
                 /* #3 VacuFormRound function */
                 {
                   0x45, /* RCVT    */
                   0x23, /* SWAP    */
                   0x46, /* GC_cur  */
                   0x60, /* ADD     */
                   0x20, /* DUP     */
                   0xB0  /* PUSHB_1 */
                         /*   38    */
                 },
                 /* #4 TTFautohint bytecode (old) */
                 {
                   0x20, /* DUP     */
                   0x64, /* ABS     */
                   0xB0, /* PUSHB_1 */
                         /*   32    */
                   0x60, /* ADD     */
                   0x66, /* FLOOR   */
                   0x23, /* SWAP    */
                   0xB0  /* PUSHB_1 */
                 },
                 /* #5 spacing function 1 */
                 {
                   0x01, /* SVTCA_x */
                   0xB0, /* PUSHB_1 */
                         /*   24    */
                   0x43, /* RS      */
                   0x58  /* IF      */
                 },
                 /* #6 spacing function 2 */
                 {
                   0x01, /* SVTCA_x */
                   0x18, /* RTG     */
                   0xB0, /* PUSHB_1 */
                         /*   24    */
                   0x43, /* RS      */
                   0x58  /* IF      */
                 },
                 /* #7 TypeMan Talk DiagEndCtrl function */
                 {
                   0x01, /* SVTCA_x */
                   0x20, /* DUP     */
                   0xB0, /* PUSHB_1 */
                         /*   3     */
                   0x25, /* CINDEX  */
                 },
                 /* #8 TypeMan Talk Align */
                 {
                   0x06, /* SPVTL   */
                   0x7D, /* RDTG    */
                 },
               };
    FT_UShort  opcode_patterns   = 9;
    FT_UShort  opcode_pointer[9] = {  0, 0, 0, 0, 0, 0, 0, 0, 0 };
    FT_UShort  opcode_size[9]    = { 12, 8, 8, 6, 7, 4, 5, 4, 2 };
    FT_UShort  i;
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */


    /* FDEF is only allowed in `prep' or `fpgm' */
    if ( exc->curRange == tt_coderange_glyph )
    {
      exc->error = FT_THROW( DEF_In_Glyf_Bytecode );
      return;
    }

    /* some font programs are broken enough to redefine functions! */
    /* We will then parse the current table.                       */

    rec   = exc->FDefs;
    limit = rec + exc->numFDefs;
    n     = (FT_ULong)args[0];

    for ( ; rec < limit; rec++ )
    {
      if ( rec->opc == n )
        break;
    }

    if ( rec == limit )
    {
      /* check that there is enough room for new functions */
      if ( exc->numFDefs >= exc->maxFDefs )
      {
        exc->error = FT_THROW( Too_Many_Function_Defs );
        return;
      }
      exc->numFDefs++;
    }

    /* Although FDEF takes unsigned 32-bit integer,  */
    /* func # must be within unsigned 16-bit integer */
    if ( n > 0xFFFFU )
    {
      exc->error = FT_THROW( Too_Many_Function_Defs );
      return;
    }

    rec->range          = exc->curRange;
    rec->opc            = (FT_UInt16)n;
    rec->start          = exc->IP + 1;
    rec->active         = TRUE;
    rec->inline_delta   = FALSE;
    rec->sph_fdef_flags = 0x0000;

    if ( n > exc->maxFunc )
      exc->maxFunc = (FT_UInt16)n;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    /* We don't know for sure these are typeman functions, */
    /* however they are only active when RS 22 is called   */
    if ( n >= 64 && n <= 66 )
      rec->sph_fdef_flags |= SPH_FDEF_TYPEMAN_STROKES;
#endif

    /* Now skip the whole function definition. */
    /* We don't allow nested IDEFS & FDEFs.    */

    while ( SkipCode( exc ) == SUCCESS )
    {

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
              FT_TRACE6(( "sph: Function %d, opcode ptrn: %d, %s %s\n",
                          i, n,
                          exc->face->root.family_name,
                          exc->face->root.style_name ));

              switch ( i )
              {
              case 0:
                rec->sph_fdef_flags             |= SPH_FDEF_INLINE_DELTA_1;
                exc->face->sph_found_func_flags |= SPH_FDEF_INLINE_DELTA_1;
                break;

              case 1:
                rec->sph_fdef_flags             |= SPH_FDEF_INLINE_DELTA_2;
                exc->face->sph_found_func_flags |= SPH_FDEF_INLINE_DELTA_2;
                break;

              case 2:
                switch ( n )
                {
                  /* needs to be implemented still */
                case 58:
                  rec->sph_fdef_flags             |= SPH_FDEF_DIAGONAL_STROKE;
                  exc->face->sph_found_func_flags |= SPH_FDEF_DIAGONAL_STROKE;
                }
                break;

              case 3:
                switch ( n )
                {
                case 0:
                  rec->sph_fdef_flags             |= SPH_FDEF_VACUFORM_ROUND_1;
                  exc->face->sph_found_func_flags |= SPH_FDEF_VACUFORM_ROUND_1;
                }
                break;

              case 4:
                /* probably not necessary to detect anymore */
                rec->sph_fdef_flags             |= SPH_FDEF_TTFAUTOHINT_1;
                exc->face->sph_found_func_flags |= SPH_FDEF_TTFAUTOHINT_1;
                break;

              case 5:
                switch ( n )
                {
                case 0:
                case 1:
                case 2:
                case 4:
                case 7:
                case 8:
                  rec->sph_fdef_flags             |= SPH_FDEF_SPACING_1;
                  exc->face->sph_found_func_flags |= SPH_FDEF_SPACING_1;
                }
                break;

              case 6:
                switch ( n )
                {
                case 0:
                case 1:
                case 2:
                case 4:
                case 7:
                case 8:
                  rec->sph_fdef_flags             |= SPH_FDEF_SPACING_2;
                  exc->face->sph_found_func_flags |= SPH_FDEF_SPACING_2;
                }
                break;

               case 7:
                 rec->sph_fdef_flags             |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
                 exc->face->sph_found_func_flags |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
                 break;

               case 8:
#if 0
                 rec->sph_fdef_flags             |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
                 exc->face->sph_found_func_flags |= SPH_FDEF_TYPEMAN_DIAGENDCTRL;
#endif
                 break;
              }
              opcode_pointer[i] = 0;
            }
          }

          else
            opcode_pointer[i] = 0;
        }

        /* Set sph_compatibility_mode only when deltas are detected */
        exc->face->sph_compatibility_mode =
          ( ( exc->face->sph_found_func_flags & SPH_FDEF_INLINE_DELTA_1 ) |
            ( exc->face->sph_found_func_flags & SPH_FDEF_INLINE_DELTA_2 ) );
      }

#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

      switch ( exc->opcode )
      {
      case 0x89:    /* IDEF */
      case 0x2C:    /* FDEF */
        exc->error = FT_THROW( Nested_DEFS );
        return;

      case 0x2D:   /* ENDF */
        rec->end = exc->IP;
        return;
      }
    }
  }
