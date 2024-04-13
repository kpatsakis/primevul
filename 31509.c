  hash_bucket( const char*  key,
               hashtable*   ht )
  {
    const char*    kp  = key;
    unsigned long  res = 0;
    hashnode*      bp  = ht->table, *ndp;


    /* Mocklisp hash function. */
    while ( *kp )
      res = ( res << 5 ) - res + *kp++;

    ndp = bp + ( res % ht->size );
    while ( *ndp )
    {
      kp = (*ndp)->key;
      if ( kp[0] == key[0] && ft_strcmp( kp, key ) == 0 )
        break;
      ndp--;
      if ( ndp < bp )
        ndp = bp + ( ht->size - 1 );
    }

    return ndp;
  }
