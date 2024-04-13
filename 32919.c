  psh_hint_table_find_strong_points( PSH_Hint_Table  table,
                                     PSH_Point       point,
                                     FT_UInt         count,
                                     FT_Int          threshold,
                                     FT_Int          major_dir )
  {
    PSH_Hint*  sort      = table->sort;
    FT_UInt    num_hints = table->num_hints;


    for ( ; count > 0; count--, point++ )
    {
      FT_Int  point_dir = 0;
      FT_Pos  org_u     = point->org_u;


      if ( psh_point_is_strong( point ) )
        continue;

      if ( PSH_DIR_COMPARE( point->dir_in, major_dir ) )
        point_dir = point->dir_in;

      else if ( PSH_DIR_COMPARE( point->dir_out, major_dir ) )
        point_dir = point->dir_out;

      if ( point_dir )
      {
        if ( point_dir == major_dir )
        {
          FT_UInt  nn;


          for ( nn = 0; nn < num_hints; nn++ )
          {
            PSH_Hint  hint = sort[nn];
            FT_Pos    d    = org_u - hint->org_pos;


            if ( d < threshold && -d < threshold )
            {
              psh_point_set_strong( point );
              point->flags2 |= PSH_POINT_EDGE_MIN;
              point->hint    = hint;
              break;
            }
          }
        }
        else if ( point_dir == -major_dir )
        {
          FT_UInt  nn;


          for ( nn = 0; nn < num_hints; nn++ )
          {
            PSH_Hint  hint = sort[nn];
            FT_Pos    d    = org_u - hint->org_pos - hint->org_len;


            if ( d < threshold && -d < threshold )
            {
              psh_point_set_strong( point );
              point->flags2 |= PSH_POINT_EDGE_MAX;
              point->hint    = hint;
              break;
            }
          }
        }
      }

#if 1
      else if ( psh_point_is_extremum( point ) )
      {
        /* treat extrema as special cases for stem edge alignment */
        FT_UInt  nn, min_flag, max_flag;


        if ( major_dir == PSH_DIR_HORIZONTAL )
        {
          min_flag = PSH_POINT_POSITIVE;
          max_flag = PSH_POINT_NEGATIVE;
        }
        else
        {
          min_flag = PSH_POINT_NEGATIVE;
          max_flag = PSH_POINT_POSITIVE;
        }

        if ( point->flags2 & min_flag )
        {
          for ( nn = 0; nn < num_hints; nn++ )
          {
            PSH_Hint  hint = sort[nn];
            FT_Pos    d    = org_u - hint->org_pos;


            if ( d < threshold && -d < threshold )
            {
              point->flags2 |= PSH_POINT_EDGE_MIN;
              point->hint    = hint;
              psh_point_set_strong( point );
              break;
            }
          }
        }
        else if ( point->flags2 & max_flag )
        {
          for ( nn = 0; nn < num_hints; nn++ )
          {
            PSH_Hint  hint = sort[nn];
            FT_Pos    d    = org_u - hint->org_pos - hint->org_len;


            if ( d < threshold && -d < threshold )
            {
              point->flags2 |= PSH_POINT_EDGE_MAX;
              point->hint    = hint;
              psh_point_set_strong( point );
              break;
            }
          }
        }

        if ( point->hint == NULL )
        {
          for ( nn = 0; nn < num_hints; nn++ )
          {
            PSH_Hint  hint = sort[nn];


            if ( org_u >= hint->org_pos                 &&
                org_u <= hint->org_pos + hint->org_len )
            {
              point->hint = hint;
              break;
            }
          }
        }
      }

#endif /* 1 */
    }
  }
