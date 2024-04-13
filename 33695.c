int dmarc_init()
{
  int *netmask   = NULL;   /* Ignored */
  int is_ipv6    = 0;
  char *tld_file = (dmarc_tld_file == NULL) ?
                   "/etc/exim/opendmarc.tlds" :
                   (char *)dmarc_tld_file;

  /* Set some sane defaults.  Also clears previous results when
   * multiple messages in one connection. */
  dmarc_pctx         = NULL;
  dmarc_status       = US"none";
  dmarc_abort        = FALSE;
  dmarc_pass_fail    = US"skipped";
  dmarc_used_domain  = US"";
  dmarc_ar_header    = NULL;
  dmarc_has_been_checked = FALSE;
  header_from_sender = NULL;
  spf_sender_domain  = NULL;
  spf_human_readable = NULL;

  /* ACLs have "control=dmarc_disable_verify" */
  if (dmarc_disable_verify == TRUE)
    return OK;

  (void) memset(&dmarc_ctx, '\0', sizeof dmarc_ctx);
  dmarc_ctx.nscount = 0;
  libdm_status = opendmarc_policy_library_init(&dmarc_ctx);
  if (libdm_status != DMARC_PARSE_OKAY)
  {
    log_write(0, LOG_MAIN|LOG_PANIC, "DMARC failure to init library: %s",
                         opendmarc_policy_status_to_str(libdm_status));
    dmarc_abort = TRUE;
  }
  if (dmarc_tld_file == NULL)
    dmarc_abort = TRUE;
  else if (opendmarc_tld_read_file(tld_file, NULL, NULL, NULL))
  {
    log_write(0, LOG_MAIN|LOG_PANIC, "DMARC failure to load tld list %s: %d",
                         tld_file, errno);
    dmarc_abort = TRUE;
  }
  if (sender_host_address == NULL)
    dmarc_abort = TRUE;
  /* This catches locally originated email and startup errors above. */
  if ( dmarc_abort == FALSE )
  {
    is_ipv6 = string_is_ip_address(sender_host_address, netmask);
    is_ipv6 = (is_ipv6 == 6) ? TRUE :
              (is_ipv6 == 4) ? FALSE : FALSE;
    dmarc_pctx = opendmarc_policy_connect_init(sender_host_address, is_ipv6);
    if (dmarc_pctx == NULL )
    {
      log_write(0, LOG_MAIN|LOG_PANIC, "DMARC failure creating policy context: ip=%s",
                                       sender_host_address);
      dmarc_abort = TRUE;
    }
  }

  return OK;
}
