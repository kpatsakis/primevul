  gray_conic_to( const FT_Vector*  control,
                 const FT_Vector*  to,
                 PWorker           worker )
  {
    gray_render_conic( RAS_VAR_ control, to );
    return 0;
  }
