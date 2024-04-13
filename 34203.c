static HB_Error  Lookup_PairPos1( GPOS_Instance*       gpi,
				  HB_PairPosFormat1*  ppf1,
				  HB_Buffer           buffer,
				  HB_UInt              first_pos,
				  HB_UShort            index,
				  HB_UShort            format1,
				  HB_UShort            format2 )
{
  HB_Error              error;
  HB_UShort             numpvr, glyph2;

  HB_PairValueRecord*  pvr;


  if ( index >= ppf1->PairSetCount )
     return ERR(HB_Err_Invalid_SubTable);

  pvr = ppf1->PairSet[index].PairValueRecord;
  if ( !pvr )
    return ERR(HB_Err_Invalid_SubTable);

  glyph2 = IN_CURGLYPH();

  for ( numpvr = ppf1->PairSet[index].PairValueCount;
	numpvr;
	numpvr--, pvr++ )
  {
    if ( glyph2 == pvr->SecondGlyph )
    {
      error = Get_ValueRecord( gpi, &pvr->Value1, format1,
			       POSITION( first_pos ) );
      if ( error )
	return error;
      return Get_ValueRecord( gpi, &pvr->Value2, format2,
			      POSITION( buffer->in_pos ) );
    }
  }

  return HB_Err_Not_Covered;
}
