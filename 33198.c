init_compress( compress_filter_context_t *zfx, z_stream *zs )
{
    int rc;
    int level;

#if defined(__riscos__) && defined(USE_ZLIBRISCOS)
    static int zlib_initialized = 0;

    if (!zlib_initialized)
        zlib_initialized = riscos_load_module("ZLib", zlib_path, 1);
#endif

    if( opt.compress_level >= 1 && opt.compress_level <= 9 )
	level = opt.compress_level;
    else if( opt.compress_level == -1 )
	level = Z_DEFAULT_COMPRESSION;
    else {
	log_error("invalid compression level; using default level\n");
	level = Z_DEFAULT_COMPRESSION;
    }

    if( (rc = zfx->algo == 1? deflateInit2( zs, level, Z_DEFLATED,
					    -13, 8, Z_DEFAULT_STRATEGY)
			    : deflateInit( zs, level )
			    ) != Z_OK ) {
	log_fatal("zlib problem: %s\n", zs->msg? zs->msg :
			       rc == Z_MEM_ERROR ? "out of core" :
			       rc == Z_VERSION_ERROR ? "invalid lib version" :
						       "unknown error" );
    }

    zfx->outbufsize = 8192;
    zfx->outbuf = xmalloc( zfx->outbufsize );
}
