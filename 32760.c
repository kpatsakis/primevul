  Ins_SCANCTRL( INS_ARG )
  {
    FT_Int  A;


    /* Get Threshold */
    A = (FT_Int)( args[0] & 0xFF );

    if ( A == 0xFF )
    {
      CUR.GS.scan_control = TRUE;
      return;
    }
    else if ( A == 0 )
    {
      CUR.GS.scan_control = FALSE;
      return;
    }

    if ( ( args[0] & 0x100 ) != 0 && CUR.tt_metrics.ppem <= A )
      CUR.GS.scan_control = TRUE;

    if ( ( args[0] & 0x200 ) != 0 && CUR.tt_metrics.rotated )
      CUR.GS.scan_control = TRUE;

    if ( ( args[0] & 0x400 ) != 0 && CUR.tt_metrics.stretched )
      CUR.GS.scan_control = TRUE;

    if ( ( args[0] & 0x800 ) != 0 && CUR.tt_metrics.ppem > A )
      CUR.GS.scan_control = FALSE;

    if ( ( args[0] & 0x1000 ) != 0 && CUR.tt_metrics.rotated )
      CUR.GS.scan_control = FALSE;

    if ( ( args[0] & 0x2000 ) != 0 && CUR.tt_metrics.stretched )
      CUR.GS.scan_control = FALSE;
  }
