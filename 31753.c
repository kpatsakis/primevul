  ftc_snode_compare( FTC_Node    ftcsnode,
                     FT_Pointer  ftcgquery,
                     FTC_Cache   cache,
                     FT_Bool*    list_changed )
  {
    FTC_SNode   snode  = (FTC_SNode)ftcsnode;
    FTC_GQuery  gquery = (FTC_GQuery)ftcgquery;
    FTC_GNode   gnode  = FTC_GNODE( snode );
    FT_UInt     gindex = gquery->gindex;
    FT_Bool     result;


    if (list_changed)
      *list_changed = FALSE;
    result = FT_BOOL( gnode->family == gquery->family                    &&
                      (FT_UInt)( gindex - gnode->gindex ) < snode->count );
    if ( result )
    {
      /* check if we need to load the glyph bitmap now */
      FTC_SBit  sbit = snode->sbits + ( gindex - gnode->gindex );


      /*
       *  The following code illustrates what to do when you want to
       *  perform operations that may fail within a lookup function.
       *
       *  Here, we want to load a small bitmap on-demand; we thus
       *  need to call the `ftc_snode_load' function which may return
       *  a non-zero error code only when we are out of memory (OOM).
       *
       *  The correct thing to do is to use @FTC_CACHE_TRYLOOP and
       *  @FTC_CACHE_TRYLOOP_END in order to implement a retry loop
       *  that is capable of flushing the cache incrementally when
       *  an OOM errors occur.
       *
       *  However, we need to `lock' the node before this operation to
       *  prevent it from being flushed within the loop.
       *
       *  When we exit the loop, we unlock the node, then check the `error'
       *  variable.  If it is non-zero, this means that the cache was
       *  completely flushed and that no usable memory was found to load
       *  the bitmap.
       *
       *  We then prefer to return a value of 0 (i.e., NO MATCH).  This
       *  ensures that the caller will try to allocate a new node.
       *  This operation consequently _fail_ and the lookup function
       *  returns the appropriate OOM error code.
       *
       *  Note that `buffer == NULL && width == 255' is a hack used to
       *  tag `unavailable' bitmaps in the array.  We should never try
       *  to load these.
       *
       */

      if ( sbit->buffer == NULL && sbit->width == 255 )
      {
        FT_ULong  size;
        FT_Error  error;


        ftcsnode->ref_count++;  /* lock node to prevent flushing */
                                /* in retry loop                 */

        FTC_CACHE_TRYLOOP( cache )
        {
          error = ftc_snode_load( snode, cache->manager, gindex, &size );
        }
        FTC_CACHE_TRYLOOP_END( list_changed );

        ftcsnode->ref_count--;  /* unlock the node */

        if ( error )
          result = 0;
        else
          cache->manager->cur_weight += size;
      }
    }

    return result;
  }
