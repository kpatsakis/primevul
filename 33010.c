  Ins_IUP( TT_ExecContext  exc )
  {
    IUP_WorkerRec  V;
    FT_Byte        mask;

    FT_UInt   first_point;   /* first point of contour        */
    FT_UInt   end_point;     /* end point (last+1) of contour */

    FT_UInt   first_touched; /* first touched point in contour   */
    FT_UInt   cur_touched;   /* current touched point in contour */

    FT_UInt   point;         /* current point   */
    FT_Short  contour;       /* current contour */


#ifdef TT_SUPPORT_SUBPIXEL_HINTING_MINIMAL
    /* See `ttinterp.h' for details on backward compatibility mode.  */
    /* Allow IUP until it has been called on both axes.  Immediately */
    /* return on subsequent ones.                                    */
    if ( SUBPIXEL_HINTING_MINIMAL    &&
         exc->backward_compatibility )
    {
      if ( exc->iupx_called && exc->iupy_called )
        return;

      if ( exc->opcode & 1 )
        exc->iupx_called = TRUE;
      else
        exc->iupy_called = TRUE;
    }
#endif

    /* ignore empty outlines */
    if ( exc->pts.n_contours == 0 )
      return;

    if ( exc->opcode & 1 )
    {
      mask   = FT_CURVE_TAG_TOUCH_X;
      V.orgs = exc->pts.org;
      V.curs = exc->pts.cur;
      V.orus = exc->pts.orus;
    }
    else
    {
      mask   = FT_CURVE_TAG_TOUCH_Y;
      V.orgs = (FT_Vector*)( (FT_Pos*)exc->pts.org + 1 );
      V.curs = (FT_Vector*)( (FT_Pos*)exc->pts.cur + 1 );
      V.orus = (FT_Vector*)( (FT_Pos*)exc->pts.orus + 1 );
    }
    V.max_points = exc->pts.n_points;

    contour = 0;
    point   = 0;

#ifdef TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY
    if ( SUBPIXEL_HINTING_INFINALITY &&
         exc->ignore_x_mode          )
    {
      exc->iup_called = TRUE;
      if ( exc->sph_tweak_flags & SPH_TWEAK_SKIP_IUP )
        return;
    }
#endif /* TT_SUPPORT_SUBPIXEL_HINTING_INFINALITY */

    do
    {
      end_point   = exc->pts.contours[contour] - exc->pts.first_point;
      first_point = point;

      if ( BOUNDS( end_point, exc->pts.n_points ) )
        end_point = exc->pts.n_points - 1;

      while ( point <= end_point && ( exc->pts.tags[point] & mask ) == 0 )
        point++;

      if ( point <= end_point )
      {
        first_touched = point;
        cur_touched   = point;

        point++;

        while ( point <= end_point )
        {
          if ( ( exc->pts.tags[point] & mask ) != 0 )
          {
            _iup_worker_interpolate( &V,
                                     cur_touched + 1,
                                     point - 1,
                                     cur_touched,
                                     point );
            cur_touched = point;
          }

          point++;
        }

        if ( cur_touched == first_touched )
          _iup_worker_shift( &V, first_point, end_point, cur_touched );
        else
        {
          _iup_worker_interpolate( &V,
                                   (FT_UShort)( cur_touched + 1 ),
                                   end_point,
                                   cur_touched,
                                   first_touched );

          if ( first_touched > 0 )
            _iup_worker_interpolate( &V,
                                     first_point,
                                     first_touched - 1,
                                     cur_touched,
                                     first_touched );
        }
      }
      contour++;
    } while ( contour < exc->pts.n_contours );
  }
