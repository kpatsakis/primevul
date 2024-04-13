static void  Free_Mark2Array( HB_Mark2Array*  m2a,
			      HB_UShort        num_classes )
{
  HB_Mark2Record  *m2r;
  HB_Anchor       *m2ans;

  HB_UNUSED(num_classes);

  if ( m2a->Mark2Record )
  {
    m2r   = m2a->Mark2Record;

    if ( m2a->Mark2Count )
    {
      m2ans = m2r[0].Mark2Anchor;
      FREE( m2ans );
    }

    FREE( m2r );
  }
}
