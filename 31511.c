  hash_insert( char*       key,
               size_t      data,
               hashtable*  ht,
               FT_Memory   memory )
  {
    hashnode  nn, *bp = hash_bucket( key, ht );
    FT_Error  error = BDF_Err_Ok;


    nn = *bp;
    if ( !nn )
    {
      if ( FT_NEW( nn ) )
        goto Exit;
      *bp = nn;

      nn->key  = key;
      nn->data = data;

      if ( ht->used >= ht->limit )
      {
        error = hash_rehash( ht, memory );
        if ( error )
          goto Exit;
      }
      ht->used++;
    }
    else
      nn->data = data;

  Exit:
    return error;
  }
