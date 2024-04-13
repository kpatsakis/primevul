  Finalize_Profile_Table( RAS_ARG )
  {
    UShort    n;
    PProfile  p;


    n = ras.num_Profs;
    p = ras.fProfile;

    if ( n > 1 && p )
    {
      while ( n > 0 )
      {
        Int  bottom, top;


        if ( n > 1 )
          p->link = (PProfile)( p->offset + p->height );
        else
          p->link = NULL;

        if ( p->flags & Flow_Up )
        {
          bottom = (Int)p->start;
          top    = (Int)( p->start + p->height - 1 );
        }
        else
        {
          bottom     = (Int)( p->start - p->height + 1 );
          top        = (Int)p->start;
          p->start   = bottom;
          p->offset += p->height - 1;
        }

        if ( Insert_Y_Turn( RAS_VARS bottom )  ||
             Insert_Y_Turn( RAS_VARS top + 1 ) )
          return FAILURE;

        p = p->link;
        n--;
      }
    }
    else
      ras.fProfile = NULL;

    return SUCCESS;
  }
