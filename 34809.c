_tls_tl_close_sockinfo (struct _tls_stream *sockinfo)
{
  if (sockinfo->socket > 0) {
    if (sockinfo->ssl_conn != NULL) {
      SSL_shutdown (sockinfo->ssl_conn);
      SSL_shutdown (sockinfo->ssl_conn);
      SSL_free (sockinfo->ssl_conn);
    }
    if (sockinfo->ssl_ctx != NULL)
      SSL_CTX_free (sockinfo->ssl_ctx);
    closesocket (sockinfo->socket);
  }
  if (sockinfo->buf != NULL)
    osip_free (sockinfo->buf);
  if (sockinfo->sendbuf != NULL)
    osip_free (sockinfo->sendbuf);
#ifdef MULTITASKING_ENABLED
  if (sockinfo->readStream != NULL) {
    CFReadStreamClose (sockinfo->readStream);
    CFRelease (sockinfo->readStream);
  }
  if (sockinfo->writeStream != NULL) {
    CFWriteStreamClose (sockinfo->writeStream);
    CFRelease (sockinfo->writeStream);
  }
#endif
  memset (sockinfo, 0, sizeof (*sockinfo));
}
