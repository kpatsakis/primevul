static HB_Error  Lookup_PairPos2( GPOS_Instance*       gpi,
				  HB_PairPosFormat2*  ppf2,
				  HB_Buffer           buffer,
				  HB_UInt              first_pos,
				  HB_UShort            format1,
				  HB_UShort            format2 )
{
  HB_Error           error;
  HB_UShort          cl1 = 0, cl2 = 0; /* shut compiler up */

  HB_Class1Record*  c1r;
  HB_Class2Record*  c2r;


  error = _HB_OPEN_Get_Class( &ppf2->ClassDef1, IN_GLYPH( first_pos ),
		     &cl1, NULL );
  if ( error && error != HB_Err_Not_Covered )
    return error;
  error = _HB_OPEN_Get_Class( &ppf2->ClassDef2, IN_CURGLYPH(),
		     &cl2, NULL );
  if ( error && error != HB_Err_Not_Covered )
    return error;

  c1r = &ppf2->Class1Record[cl1];
  if ( !c1r )
    return ERR(HB_Err_Invalid_SubTable);
  c2r = &c1r->Class2Record[cl2];

  error = Get_ValueRecord( gpi, &c2r->Value1, format1, POSITION( first_pos ) );
  if ( error )
    return error;
  return Get_ValueRecord( gpi, &c2r->Value2, format2, POSITION( buffer->in_pos ) );
}
