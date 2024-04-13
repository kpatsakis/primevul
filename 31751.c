  FTC_SNode_New( FTC_SNode  *psnode,
                 FTC_GQuery  gquery,
                 FTC_Cache   cache )
  {
    FT_Memory   memory = cache->memory;
    FT_Error    error;
    FTC_SNode   snode  = NULL;
    FT_UInt     gindex = gquery->gindex;
    FTC_Family  family = gquery->family;

    FTC_SFamilyClass  clazz = FTC_CACHE__SFAMILY_CLASS( cache );
    FT_UInt           total;
    FT_UInt           node_count;


    total = clazz->family_get_count( family, cache->manager );
    if ( total == 0 || gindex >= total )
    {
      error = FT_THROW( Invalid_Argument );
      goto Exit;
    }

    if ( !FT_NEW( snode ) )
    {
      FT_UInt  count, start;


      start = gindex - ( gindex % FTC_SBIT_ITEMS_PER_NODE );
      count = total - start;
      if ( count > FTC_SBIT_ITEMS_PER_NODE )
        count = FTC_SBIT_ITEMS_PER_NODE;

      FTC_GNode_Init( FTC_GNODE( snode ), start, family );

      snode->count = count;
      for ( node_count = 0; node_count < count; node_count++ )
      {
        snode->sbits[node_count].width = 255;
      }

      error = ftc_snode_load( snode,
                              cache->manager,
                              gindex,
                              NULL );
      if ( error )
      {
        FTC_SNode_Free( snode, cache );
        snode = NULL;
      }
    }

  Exit:
    *psnode = snode;
    return error;
  }
