  hash_rehash( hashtable*  ht,
               FT_Memory   memory )
  {
    hashnode*  obp = ht->table, *bp, *nbp;
    int        i, sz = ht->size;
    FT_Error   error = BDF_Err_Ok;


    ht->size <<= 1;
    ht->limit  = ht->size / 3;

    if ( FT_NEW_ARRAY( ht->table, ht->size ) )
      goto Exit;

    for ( i = 0, bp = obp; i < sz; i++, bp++ )
    {
      if ( *bp )
      {
        nbp = hash_bucket( (*bp)->key, ht );
        *nbp = *bp;
      }
    }
    FT_FREE( obp );

  Exit:
    return error;
  }
