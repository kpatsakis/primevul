  tt_synth_sfnt_checksum( FT_Stream  stream,
                          FT_ULong   length )
  {
    FT_Error   error;
    FT_UInt32  checksum = 0;
    FT_UInt    i;


    if ( FT_FRAME_ENTER( length ) )
      return 0;

    for ( ; length > 3; length -= 4 )
      checksum += (FT_UInt32)FT_GET_ULONG();

    for ( i = 3; length > 0; length--, i-- )
      checksum += (FT_UInt32)FT_GET_BYTE() << ( i * 8 );

    FT_FRAME_EXIT();

    return checksum;
  }
