  ftc_snode_new( FTC_Node   *ftcpsnode,
                 FT_Pointer  ftcgquery,
                 FTC_Cache   cache )
  {
    FTC_SNode  *psnode = (FTC_SNode*)ftcpsnode;
    FTC_GQuery  gquery = (FTC_GQuery)ftcgquery;


    return FTC_SNode_New( psnode, gquery, cache );
  }
