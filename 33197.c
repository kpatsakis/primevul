handle_compressed( void *procctx, PKT_compressed *cd,
		   int (*callback)(IOBUF, void *), void *passthru )
{
    compress_filter_context_t *cfx;
    int rc;

    if(check_compress_algo(cd->algorithm))
      return G10ERR_COMPR_ALGO;
    cfx = xmalloc_clear (sizeof *cfx);
    cfx->release = release_context;
    cfx->algo = cd->algorithm;
    push_compress_filter(cd->buf,cfx,cd->algorithm);
    if( callback )
	rc = callback(cd->buf, passthru );
    else
	rc = proc_packets(procctx, cd->buf);
    cd->buf = NULL;
    return rc;
}
