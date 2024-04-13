  Ins_SCANCTRL( TT_ExecContext  exc,
                FT_Long*        args )
  {
    FT_Int  A;


    /* Get Threshold */
    A = (FT_Int)( args[0] & 0xFF );

    if ( A == 0xFF )
    {
      exc->GS.scan_control = TRUE;
      return;
    }
    else if ( A == 0 )
    {
      exc->GS.scan_control = FALSE;
      return;
    }

    if ( ( args[0] & 0x100 ) != 0 && exc->tt_metrics.ppem <= A )
      exc->GS.scan_control = TRUE;

    if ( ( args[0] & 0x200 ) != 0 && exc->tt_metrics.rotated )
      exc->GS.scan_control = TRUE;

    if ( ( args[0] & 0x400 ) != 0 && exc->tt_metrics.stretched )
      exc->GS.scan_control = TRUE;

    if ( ( args[0] & 0x800 ) != 0 && exc->tt_metrics.ppem > A )
      exc->GS.scan_control = FALSE;

    if ( ( args[0] & 0x1000 ) != 0 && exc->tt_metrics.rotated )
      exc->GS.scan_control = FALSE;

    if ( ( args[0] & 0x2000 ) != 0 && exc->tt_metrics.stretched )
      exc->GS.scan_control = FALSE;
  }
