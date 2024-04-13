static HB_Error  Do_ContextPos( GPOS_Instance*        gpi,
				HB_UShort             GlyphCount,
				HB_UShort             PosCount,
				HB_PosLookupRecord*  pos,
				HB_Buffer            buffer,
				int                   nesting_level )
{
  HB_Error  error;
  HB_UInt   i, old_pos;


  i = 0;

  while ( i < GlyphCount )
  {
    if ( PosCount && i == pos->SequenceIndex )
    {
      old_pos = buffer->in_pos;

      /* Do a positioning */

      error = GPOS_Do_Glyph_Lookup( gpi, pos->LookupListIndex, buffer,
				    GlyphCount, nesting_level );

      if ( error )
	return error;

      pos++;
      PosCount--;
      i += buffer->in_pos - old_pos;
    }
    else
    {
      i++;
      (buffer->in_pos)++;
    }
  }

  return HB_Err_Ok;
}
