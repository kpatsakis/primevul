  ftc_snode_weight( FTC_Node   ftcsnode,
                    FTC_Cache  cache )
  {
    FTC_SNode  snode = (FTC_SNode)ftcsnode;
    FT_UInt    count = snode->count;
    FTC_SBit   sbit  = snode->sbits;
    FT_Int     pitch;
    FT_Offset  size;

    FT_UNUSED( cache );


    FT_ASSERT( snode->count <= FTC_SBIT_ITEMS_PER_NODE );

    /* the node itself */
    size = sizeof ( *snode );

    for ( ; count > 0; count--, sbit++ )
    {
      if ( sbit->buffer )
      {
        pitch = sbit->pitch;
        if ( pitch < 0 )
          pitch = -pitch;

        /* add the size of a given glyph image */
        size += pitch * sbit->height;
      }
    }

    return size;
  }
