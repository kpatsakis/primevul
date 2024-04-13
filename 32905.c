  psh_glyph_compute_inflections( PSH_Glyph  glyph )
  {
    FT_UInt  n;


    for ( n = 0; n < glyph->num_contours; n++ )
    {
      PSH_Point  first, start, end, before, after;
      FT_Pos     in_x, in_y, out_x, out_y;
      FT_Int     orient_prev, orient_cur;
      FT_Int     finished = 0;


      /* we need at least 4 points to create an inflection point */
      if ( glyph->contours[n].count < 4 )
        continue;

      /* compute first segment in contour */
      first = glyph->contours[n].start;

      start = end = first;
      do
      {
        end = end->next;
        if ( end == first )
          goto Skip;

        in_x = end->org_u - start->org_u;
        in_y = end->org_v - start->org_v;

      } while ( in_x == 0 && in_y == 0 );

      /* extend the segment start whenever possible */
      before = start;
      do
      {
        do
        {
          start  = before;
          before = before->prev;
          if ( before == first )
            goto Skip;

          out_x = start->org_u - before->org_u;
          out_y = start->org_v - before->org_v;

        } while ( out_x == 0 && out_y == 0 );

        orient_prev = psh_corner_orientation( in_x, in_y, out_x, out_y );

      } while ( orient_prev == 0 );

      first = start;
      in_x  = out_x;
      in_y  = out_y;

      /* now, process all segments in the contour */
      do
      {
        /* first, extend current segment's end whenever possible */
        after = end;
        do
        {
          do
          {
            end   = after;
            after = after->next;
            if ( after == first )
              finished = 1;

            out_x = after->org_u - end->org_u;
            out_y = after->org_v - end->org_v;

          } while ( out_x == 0 && out_y == 0 );

          orient_cur = psh_corner_orientation( in_x, in_y, out_x, out_y );

        } while ( orient_cur == 0 );

        if ( ( orient_cur ^ orient_prev ) < 0 )
        {
          do
          {
            psh_point_set_inflex( start );
            start = start->next;
          }
          while ( start != end );

          psh_point_set_inflex( start );
        }

        start       = end;
        end         = after;
        orient_prev = orient_cur;
        in_x        = out_x;
        in_y        = out_y;

      } while ( !finished );

    Skip:
      ;
    }
  }
