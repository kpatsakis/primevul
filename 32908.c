  psh_glyph_interpolate_normal_points( PSH_Glyph  glyph,
                                       FT_Int     dimension )
  {

#if 1
    /* first technique: a point is strong if it is a local extremum */

    PSH_Dimension  dim    = &glyph->globals->dimension[dimension];
    FT_Fixed       scale  = dim->scale_mult;
    FT_Memory      memory = glyph->memory;

    PSH_Point*     strongs     = NULL;
    PSH_Point      strongs_0[PSH_MAX_STRONG_INTERNAL];
    FT_UInt        num_strongs = 0;

    PSH_Point      points = glyph->points;
    PSH_Point      points_end = points + glyph->num_points;
    PSH_Point      point;


    /* first count the number of strong points */
    for ( point = points; point < points_end; point++ )
    {
      if ( psh_point_is_strong( point ) )
        num_strongs++;
    }

    if ( num_strongs == 0 )  /* nothing to do here */
      return;

    /* allocate an array to store a list of points, */
    /* stored in increasing org_u order             */
    if ( num_strongs <= PSH_MAX_STRONG_INTERNAL )
      strongs = strongs_0;
    else
    {
      FT_Error  error;


      if ( FT_NEW_ARRAY( strongs, num_strongs ) )
        return;
    }

    num_strongs = 0;
    for ( point = points; point < points_end; point++ )
    {
      PSH_Point*  insert;


      if ( !psh_point_is_strong( point ) )
        continue;

      for ( insert = strongs + num_strongs; insert > strongs; insert-- )
      {
        if ( insert[-1]->org_u <= point->org_u )
          break;

        insert[0] = insert[-1];
      }
      insert[0] = point;
      num_strongs++;
    }

    /* now try to interpolate all normal points */
    for ( point = points; point < points_end; point++ )
    {
      if ( psh_point_is_strong( point ) )
        continue;

      /* sometimes, some local extrema are smooth points */
      if ( psh_point_is_smooth( point ) )
      {
        if ( point->dir_in == PSH_DIR_NONE   ||
             point->dir_in != point->dir_out )
          continue;

        if ( !psh_point_is_extremum( point ) &&
             !psh_point_is_inflex( point )   )
          continue;

        point->flags &= ~PSH_POINT_SMOOTH;
      }

      /* find best enclosing point coordinates then interpolate */
      {
        PSH_Point   before, after;
        FT_UInt     nn;


        for ( nn = 0; nn < num_strongs; nn++ )
          if ( strongs[nn]->org_u > point->org_u )
            break;

        if ( nn == 0 )  /* point before the first strong point */
        {
          after = strongs[0];

          point->cur_u = after->cur_u +
                           FT_MulFix( point->org_u - after->org_u,
                                      scale );
        }
        else
        {
          before = strongs[nn - 1];

          for ( nn = num_strongs; nn > 0; nn-- )
            if ( strongs[nn - 1]->org_u < point->org_u )
              break;

          if ( nn == num_strongs )  /* point is after last strong point */
          {
            before = strongs[nn - 1];

            point->cur_u = before->cur_u +
                             FT_MulFix( point->org_u - before->org_u,
                                        scale );
          }
          else
          {
            FT_Pos  u;


            after = strongs[nn];

            /* now interpolate point between before and after */
            u = point->org_u;

            if ( u == before->org_u )
              point->cur_u = before->cur_u;

            else if ( u == after->org_u )
              point->cur_u = after->cur_u;

            else
              point->cur_u = before->cur_u +
                               FT_MulDiv( u - before->org_u,
                                          after->cur_u - before->cur_u,
                                          after->org_u - before->org_u );
          }
        }
        psh_point_set_fitted( point );
      }
    }

    if ( strongs != strongs_0 )
      FT_FREE( strongs );

#endif /* 1 */

  }
