  cid_get_offset( FT_Byte*  *start,
                  FT_Byte    offsize )
  {
    FT_ULong  result;
    FT_Byte*  p = *start;


    for ( result = 0; offsize > 0; offsize-- )
    {
      result <<= 8;
      result  |= *p++;
    }

    *start = p;
    return (FT_Long)result;
  }
