static HB_Error  Lookup_CursivePos( GPOS_Instance*    gpi,
				    HB_GPOS_SubTable* st,
				    HB_Buffer        buffer,
				    HB_UShort         flags,
				    HB_UShort         context_length,
				    int               nesting_level )
{
  HB_UShort        index, property;
  HB_Error         error;
  HB_GPOSHeader*  gpos = gpi->gpos;
  HB_CursivePos*  cp = &st->cursive;

  HB_EntryExitRecord*  eer;
  HB_Fixed                entry_x, entry_y;
  HB_Fixed                exit_x, exit_y;

  HB_UNUSED(nesting_level);

  if ( context_length != 0xFFFF && context_length < 1 )
  {
    gpi->last = 0xFFFF;
    return HB_Err_Not_Covered;
  }

  /* Glyphs not having the right GDEF properties will be ignored, i.e.,
     gpi->last won't be reset (contrary to user defined properties). */

  if ( CHECK_Property( gpos->gdef, IN_CURITEM(), flags, &property ) )
    return error;

  /* We don't handle mark glyphs here.  According to Andrei, this isn't
     possible, but who knows...                                         */

  if ( property == HB_GDEF_MARK )
  {
    gpi->last = 0xFFFF;
    return HB_Err_Not_Covered;
  }

  error = _HB_OPEN_Coverage_Index( &cp->Coverage, IN_CURGLYPH(), &index );
  if ( error )
  {
    gpi->last = 0xFFFF;
    return error;
  }

  if ( index >= cp->EntryExitCount )
    return ERR(HB_Err_Invalid_SubTable);

  eer = &cp->EntryExitRecord[index];

  /* Now comes the messiest part of the whole OpenType
     specification.  At first glance, cursive connections seem easy
     to understand, but there are pitfalls!  The reason is that
     the specs don't mention how to compute the advance values
     resp. glyph offsets.  I was told it would be an omission, to
     be fixed in the next OpenType version...  Again many thanks to
     Andrei Burago <andreib@microsoft.com> for clarifications.

     Consider the following example:

		      |  xadv1    |
		       +---------+
		       |         |
		 +-----+--+ 1    |
		 |     | .|      |
		 |    0+--+------+
		 |   2    |
		 |        |
		0+--------+
		|  xadv2   |

       glyph1: advance width = 12
	       anchor point = (3,1)

       glyph2: advance width = 11
	       anchor point = (9,4)

       LSB is 1 for both glyphs (so the boxes drawn above are glyph
       bboxes).  Writing direction is R2L; `0' denotes the glyph's
       coordinate origin.

     Now the surprising part: The advance width of the *left* glyph
     (resp. of the *bottom* glyph) will be modified, no matter
     whether the writing direction is L2R or R2L (resp. T2B or
     B2T)!  This assymetry is caused by the fact that the glyph's
     coordinate origin is always the lower left corner for all
     writing directions.

     Continuing the above example, we can compute the new
     (horizontal) advance width of glyph2 as

       9 - 3 = 6  ,

     and the new vertical offset of glyph2 as

       1 - 4 = -3  .


     Vertical writing direction is far more complicated:

     a) Assuming that we recompute the advance height of the lower glyph:

				  --
		       +---------+
	      --       |         |
		 +-----+--+ 1    | yadv1
		 |     | .|      |
	   yadv2 |    0+--+------+        -- BSB1  --
		 |   2    |       --      --        y_offset
		 |        |
   BSB2 --      0+--------+                        --
	--    --

       glyph1: advance height = 6
	       anchor point = (3,1)

       glyph2: advance height = 7
	       anchor point = (9,4)

       TSB is 1 for both glyphs; writing direction is T2B.


	 BSB1     = yadv1 - (TSB1 + ymax1)
	 BSB2     = yadv2 - (TSB2 + ymax2)
	 y_offset = y2 - y1

       vertical advance width of glyph2
	 = y_offset + BSB2 - BSB1
	 = (y2 - y1) + (yadv2 - (TSB2 + ymax2)) - (yadv1 - (TSB1 + ymax1))
	 = y2 - y1 + yadv2 - TSB2 - ymax2 - (yadv1 - TSB1 - ymax1)
	 = y2 - y1 + yadv2 - TSB2 - ymax2 - yadv1 + TSB1 + ymax1


     b) Assuming that we recompute the advance height of the upper glyph:

				  --      --
		       +---------+        -- TSB1
	--    --       |         |
   TSB2 --       +-----+--+ 1    | yadv1   ymax1
		 |     | .|      |
	   yadv2 |    0+--+------+        --       --
    ymax2        |   2    |       --                y_offset
		 |        |
	--      0+--------+                        --
	      --

       glyph1: advance height = 6
	       anchor point = (3,1)

       glyph2: advance height = 7
	       anchor point = (9,4)

       TSB is 1 for both glyphs; writing direction is T2B.

       y_offset = y2 - y1

       vertical advance width of glyph2
	 = TSB1 + ymax1 + y_offset - (TSB2 + ymax2)
	 = TSB1 + ymax1 + y2 - y1 - TSB2 - ymax2


     Comparing a) with b) shows that b) is easier to compute.  I'll wait
     for a reply from Andrei to see what should really be implemented...

     Since horizontal advance widths or vertical advance heights
     can be used alone but not together, no ambiguity occurs.        */

  if ( gpi->last == 0xFFFF )
    goto end;

  /* Get_Anchor() returns HB_Err_Not_Covered if there is no anchor
     table.                                                         */

  error = Get_Anchor( gpi, &eer->EntryAnchor, IN_CURGLYPH(),
		      &entry_x, &entry_y );
  if ( error == HB_Err_Not_Covered )
    goto end;
  if ( error )
    return error;

  if ( gpi->r2l )
  {
    POSITION( buffer->in_pos )->x_advance   = entry_x - gpi->anchor_x;
    POSITION( buffer->in_pos )->new_advance = TRUE;
  }
  else
  {
    POSITION( gpi->last )->x_advance   = gpi->anchor_x - entry_x;
    POSITION( gpi->last )->new_advance = TRUE;
  }

  if ( flags & HB_LOOKUP_FLAG_RIGHT_TO_LEFT )
  {
    POSITION( gpi->last )->cursive_chain = gpi->last - buffer->in_pos;
    POSITION( gpi->last )->y_pos = entry_y - gpi->anchor_y;
  }
  else
  {
    POSITION( buffer->in_pos )->cursive_chain = buffer->in_pos - gpi->last;
    POSITION( buffer->in_pos )->y_pos = gpi->anchor_y - entry_y;
  }

end:
  error = Get_Anchor( gpi, &eer->ExitAnchor, IN_CURGLYPH(),
		      &exit_x, &exit_y );
  if ( error == HB_Err_Not_Covered )
    gpi->last = 0xFFFF;
  else
  {
    gpi->last     = buffer->in_pos;
    gpi->anchor_x = exit_x;
    gpi->anchor_y = exit_y;
  }
  if ( error )
    return error;

  (buffer->in_pos)++;

  return HB_Err_Ok;
}
