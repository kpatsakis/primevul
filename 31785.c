  Vertical_Gray_Sweep_Step( RAS_ARG )
  {
    short*  count = (short*)count_table;
    Byte*   grays;


    ras.traceOfs += ras.gray_width;

    if ( ras.traceOfs > ras.gray_width )
    {
      PByte  pix;


      pix   = ras.gTarget + ras.traceG + ras.gray_min_x * 4;
      grays = ras.grays;

      if ( ras.gray_max_x >= 0 )
      {
        Long  last_pixel = ras.target.width - 1;
        Int   last_cell  = last_pixel >> 2;
        Int   last_bit   = last_pixel & 3;
        Bool  over       = 0;

        Int    c1, c2;
        PByte  bit, bit2;


        if ( ras.gray_max_x >= last_cell && last_bit != 3 )
        {
          ras.gray_max_x = last_cell - 1;
          over = 1;
        }

        if ( ras.gray_min_x < 0 )
          ras.gray_min_x = 0;

        bit  = ras.bTarget + ras.gray_min_x;
        bit2 = bit + ras.gray_width;

        c1 = ras.gray_max_x - ras.gray_min_x;

        while ( c1 >= 0 )
        {
          c2 = count[*bit] + count[*bit2];

          if ( c2 )
          {
            pix[0] = grays[(c2 >> 12) & 0x000F];
            pix[1] = grays[(c2 >> 8 ) & 0x000F];
            pix[2] = grays[(c2 >> 4 ) & 0x000F];
            pix[3] = grays[ c2        & 0x000F];

            *bit  = 0;
            *bit2 = 0;
          }

          bit++;
          bit2++;
          pix += 4;
          c1--;
        }

        if ( over )
        {
          c2 = count[*bit] + count[*bit2];
          if ( c2 )
          {
            switch ( last_bit )
            {
            case 2:
              pix[2] = grays[(c2 >> 4 ) & 0x000F];
            case 1:
              pix[1] = grays[(c2 >> 8 ) & 0x000F];
            default:
              pix[0] = grays[(c2 >> 12) & 0x000F];
            }

            *bit  = 0;
            *bit2 = 0;
          }
        }
      }

      ras.traceOfs = 0;
      ras.traceG  += ras.traceIncr;

      ras.gray_min_x =  32000;
      ras.gray_max_x = -32000;
    }
  }
