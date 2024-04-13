  hash_lookup( const char* key,
               hashtable*  ht )
  {
    hashnode *np = hash_bucket( key, ht );


    return *np;
  }
