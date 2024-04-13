  Set_High_Precision( RAS_ARGS Int  High )
  {
    /*
     * `precision_step' is used in `Bezier_Up' to decide when to split a
     * given y-monotonous Bezier arc that crosses a scanline before
     * approximating it as a straight segment.  The default value of 32 (for
     * low accuracy) corresponds to
     *
     *   32 / 64 == 0.5 pixels ,
     *
     * while for the high accuracy case we have
     *
     *   256/ (1 << 12) = 0.0625 pixels .
     *
     * `precision_jitter' is an epsilon threshold used in
     * `Vertical_Sweep_Span' to deal with small imperfections in the Bezier
     * decomposition (after all, we are working with approximations only);
     * it avoids switching on additional pixels which would cause artifacts
     * otherwise.
     *
     * The value of `precision_jitter' has been determined heuristically.
     *
     */

    if ( High )
    {
      ras.precision_bits   = 12;
      ras.precision_step   = 256;
      ras.precision_jitter = 30;
    }
    else
    {
      ras.precision_bits   = 6;
      ras.precision_step   = 32;
      ras.precision_jitter = 2;
    }

    FT_TRACE6(( "Set_High_Precision(%s)\n", High ? "true" : "false" ));

    ras.precision       = 1 << ras.precision_bits;
    ras.precision_half  = ras.precision / 2;
    ras.precision_shift = ras.precision_bits - Pixel_Bits;
  }
