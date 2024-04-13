  mm_weights_unmap( FT_Fixed*  weights,
                    FT_Fixed*  axiscoords,
                    FT_UInt    axis_count )
  {
    FT_ASSERT( axis_count <= T1_MAX_MM_AXIS );

    if ( axis_count == 1 )
      axiscoords[0] = weights[1];

    else if ( axis_count == 2 )
    {
      axiscoords[0] = weights[3] + weights[1];
      axiscoords[1] = weights[3] + weights[2];
    }

    else if ( axis_count == 3 )
    {
      axiscoords[0] = weights[7] + weights[5] + weights[3] + weights[1];
      axiscoords[1] = weights[7] + weights[6] + weights[3] + weights[2];
      axiscoords[2] = weights[7] + weights[6] + weights[5] + weights[4];
    }

    else
    {
      axiscoords[0] = weights[15] + weights[13] + weights[11] + weights[9] +
                        weights[7] + weights[5] + weights[3] + weights[1];
      axiscoords[1] = weights[15] + weights[14] + weights[11] + weights[10] +
                        weights[7] + weights[6] + weights[3] + weights[2];
      axiscoords[2] = weights[15] + weights[14] + weights[13] + weights[12] +
                        weights[7] + weights[6] + weights[5] + weights[4];
      axiscoords[3] = weights[15] + weights[14] + weights[13] + weights[12] +
                        weights[11] + weights[10] + weights[9] + weights[8];
    }
  }
