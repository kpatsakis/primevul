  ft_black_reset( black_PRaster  raster,
                  char*          pool_base,
                  long           pool_size )
  {
    if ( raster )
    {
      if ( pool_base && pool_size >= (long)sizeof ( black_TWorker ) + 2048 )
      {
        black_PWorker  worker = (black_PWorker)pool_base;


        raster->buffer      = pool_base + ( ( sizeof ( *worker ) + 7 ) & ~7 );
        raster->buffer_size = (long)( pool_base + pool_size -
                                        (char*)raster->buffer );
        raster->worker      = worker;
      }
      else
      {
        raster->buffer      = NULL;
        raster->buffer_size = 0;
        raster->worker      = NULL;
      }
    }
  }
