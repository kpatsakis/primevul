  read_pfb_tag( FT_Stream   stream,
                FT_UShort  *atag,
                FT_ULong   *asize )
  {
    FT_Error   error;
    FT_UShort  tag;
    FT_ULong   size;


    *atag  = 0;
    *asize = 0;

    if ( !FT_READ_USHORT( tag ) )
    {
      if ( tag == 0x8001U || tag == 0x8002U )
      {
        if ( !FT_READ_ULONG_LE( size ) )
          *asize = size;
      }

      *atag = tag;
    }

    return error;
  }
