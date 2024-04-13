  ftc_snode_free( FTC_Node   ftcsnode,
                  FTC_Cache  cache )
  {
    FTC_SNode  snode  = (FTC_SNode)ftcsnode;
    FTC_SBit   sbit   = snode->sbits;
    FT_UInt    count  = snode->count;
    FT_Memory  memory = cache->memory;


    for ( ; count > 0; sbit++, count-- )
      FT_FREE( sbit->buffer );

    FTC_GNode_Done( FTC_GNODE( snode ), cache );

    FT_FREE( snode );
  }
