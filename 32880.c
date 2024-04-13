  gray_cubic_to( const FT_Vector*  control1,
                 const FT_Vector*  control2,
                 const FT_Vector*  to,
                 PWorker           worker )
  {
    gray_render_cubic( RAS_VAR_ control1, control2, to );
    return 0;
  }
