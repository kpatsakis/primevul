push_compress_filter2(IOBUF out,compress_filter_context_t *zfx,
		      int algo,int rel)
{
  if(algo>=0)
    zfx->algo=algo;
  else
    zfx->algo=DEFAULT_COMPRESS_ALGO;

  switch(zfx->algo)
    {
    case COMPRESS_ALGO_NONE:
      break;

    case COMPRESS_ALGO_ZIP:
    case COMPRESS_ALGO_ZLIB:
      iobuf_push_filter2(out,compress_filter,zfx,rel);
      break;

#ifdef HAVE_BZIP2
    case COMPRESS_ALGO_BZIP2:
      iobuf_push_filter2(out,compress_filter_bz2,zfx,rel);
      break;
#endif

    default:
      BUG();
    }
}
