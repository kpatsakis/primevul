  Ins_MPS( TT_ExecContext  exc,
           FT_Long*        args )
  {
    if ( NO_SUBPIXEL_HINTING )
    {
      /* Microsoft's GDI bytecode interpreter always returns value 12; */
      /* we return the current PPEM value instead.                     */
      args[0] = exc->func_cur_ppem( exc );
    }
    else
    {
      /* A possible practical application of the MPS instruction is to   */
      /* implement optical scaling and similar features, which should be */
      /* based on perceptual attributes, thus independent of the         */
      /* resolution.                                                     */
      args[0] = exc->pointSize;
    }
  }
