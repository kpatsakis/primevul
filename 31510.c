  hash_init( hashtable*  ht,
             FT_Memory   memory )
  {
    int       sz = INITIAL_HT_SIZE;
    FT_Error  error = BDF_Err_Ok;


    ht->size  = sz;
    ht->limit = sz / 3;
    ht->used  = 0;

    if ( FT_NEW_ARRAY( ht->table, sz ) )
      goto Exit;

  Exit:
    return error;
  }
