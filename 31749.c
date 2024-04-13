  FTC_SNode_Compare( FTC_SNode   snode,
                     FTC_GQuery  gquery,
                     FTC_Cache   cache,
                     FT_Bool*    list_changed )
  {
    return ftc_snode_compare( FTC_NODE( snode ), gquery,
                              cache, list_changed );
  }
