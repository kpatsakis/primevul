  gray_line_to( const FT_Vector*  to,
                PWorker           worker )
  {
    gray_render_line( RAS_VAR_ UPSCALE( to->x ), UPSCALE( to->y ) );
    return 0;
  }
