static HB_Error  Load_PosClassRule( HB_ContextPosFormat2*  cpf2,
				    HB_PosClassRule*       pcr,
				    HB_Stream               stream )
{
  HB_Error  error;

  HB_UShort             n, count;

  HB_UShort*            c;
  HB_PosLookupRecord*  plr;


  if ( ACCESS_Frame( 4L ) )
    return error;

  pcr->GlyphCount = GET_UShort();
  pcr->PosCount   = GET_UShort();

  FORGET_Frame();

  if ( pcr->GlyphCount > cpf2->MaxContextLength )
    cpf2->MaxContextLength = pcr->GlyphCount;

  pcr->Class = NULL;

  count = pcr->GlyphCount - 1;        /* only GlyphCount - 1 elements */

  if ( ALLOC_ARRAY( pcr->Class, count, HB_UShort ) )
    return error;

  c = pcr->Class;

  if ( ACCESS_Frame( count * 2L ) )
    goto Fail2;

  for ( n = 0; n < count; n++ )
    c[n] = GET_UShort();

  FORGET_Frame();

  pcr->PosLookupRecord = NULL;

  count = pcr->PosCount;

  if ( ALLOC_ARRAY( pcr->PosLookupRecord, count, HB_PosLookupRecord ) )
    goto Fail2;

  plr = pcr->PosLookupRecord;

  if ( ACCESS_Frame( count * 4L ) )
    goto Fail1;

  for ( n = 0; n < count; n++ )
  {
    plr[n].SequenceIndex   = GET_UShort();
    plr[n].LookupListIndex = GET_UShort();
  }

  FORGET_Frame();

  return HB_Err_Ok;

Fail1:
  FREE( plr );

Fail2:
  FREE( c );
  return error;
}
