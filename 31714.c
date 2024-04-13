  pcf_get_metrics( FT_Stream  stream,
                   PCF_Face   face )
  {
    FT_Error    error;
    FT_Memory   memory  = FT_FACE( face )->memory;
    FT_ULong    format, size;
    PCF_Metric  metrics = 0;
    FT_ULong    nmetrics, i;


    error = pcf_seek_to_table_type( stream,
                                    face->toc.tables,
                                    face->toc.count,
                                    PCF_METRICS,
                                    &format,
                                    &size );
    if ( error )
      return error;

    if ( FT_READ_ULONG_LE( format ) )
      goto Bail;

    if ( !PCF_FORMAT_MATCH( format, PCF_DEFAULT_FORMAT )     &&
         !PCF_FORMAT_MATCH( format, PCF_COMPRESSED_METRICS ) )
      return FT_THROW( Invalid_File_Format );

    if ( PCF_FORMAT_MATCH( format, PCF_DEFAULT_FORMAT ) )
    {
      if ( PCF_BYTE_ORDER( format ) == MSBFirst )
        (void)FT_READ_ULONG( nmetrics );
      else
        (void)FT_READ_ULONG_LE( nmetrics );
    }
    else
    {
      if ( PCF_BYTE_ORDER( format ) == MSBFirst )
        (void)FT_READ_USHORT( nmetrics );
      else
        (void)FT_READ_USHORT_LE( nmetrics );
    }
    if ( error )
      return FT_THROW( Invalid_File_Format );

    face->nmetrics = nmetrics;

    if ( !nmetrics )
      return FT_THROW( Invalid_Table );

    FT_TRACE4(( "pcf_get_metrics:\n" ));

    FT_TRACE4(( "  number of metrics: %d\n", nmetrics ));

    /* rough estimate */
    if ( PCF_FORMAT_MATCH( format, PCF_DEFAULT_FORMAT ) )
    {
      if ( nmetrics > size / PCF_METRIC_SIZE )
        return FT_THROW( Invalid_Table );
    }
    else
    {
      if ( nmetrics > size / PCF_COMPRESSED_METRIC_SIZE )
        return FT_THROW( Invalid_Table );
    }

    if ( FT_NEW_ARRAY( face->metrics, nmetrics ) )
      return FT_THROW( Out_Of_Memory );

    metrics = face->metrics;
    for ( i = 0; i < nmetrics; i++ )
    {
      error = pcf_get_metric( stream, format, metrics + i );

      metrics[i].bits = 0;

      FT_TRACE5(( "  idx %d: width=%d, "
                  "lsb=%d, rsb=%d, ascent=%d, descent=%d, swidth=%d\n",
                  i,
                  ( metrics + i )->characterWidth,
                  ( metrics + i )->leftSideBearing,
                  ( metrics + i )->rightSideBearing,
                  ( metrics + i )->ascent,
                  ( metrics + i )->descent,
                  ( metrics + i )->attributes ));

      if ( error )
        break;
    }

    if ( error )
      FT_FREE( face->metrics );

  Bail:
    return error;
  }
