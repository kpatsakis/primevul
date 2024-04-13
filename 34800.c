eXosip_quit (struct eXosip_t *excontext)
{
  jauthinfo_t *jauthinfo;
  eXosip_call_t *jc;
  eXosip_reg_t *jreg;

#ifndef MINISIZE
  eXosip_notify_t *jn;
  eXosip_subscribe_t *js;
  eXosip_pub_t *jpub;
#endif
#ifndef OSIP_MONOTHREAD
  int i;
#endif

  if (excontext == NULL)
    return;

  if (excontext->j_stop_ua == -1) {
    OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_WARNING, NULL, "eXosip: already stopped!\n"));
    return;
  }

  excontext->j_stop_ua = 1;     /* ask to quit the application */
  _eXosip_wakeup (excontext);
  eXosip_wakeup_event (excontext);

#ifndef OSIP_MONOTHREAD
  if (excontext->j_thread != NULL) {
    i = osip_thread_join ((struct osip_thread *) excontext->j_thread);
    if (i != 0) {
      OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_ERROR, NULL, "eXosip: can't terminate thread!\n"));
    }
    osip_free ((struct osip_thread *) excontext->j_thread);
  }

  jpipe_close (excontext->j_socketctl);
  jpipe_close (excontext->j_socketctl_event);
#endif

  osip_free (excontext->user_agent);

  for (jc = excontext->j_calls; jc != NULL; jc = excontext->j_calls) {
    REMOVE_ELEMENT (excontext->j_calls, jc);
    _eXosip_call_free (excontext, jc);
  }

#ifndef MINISIZE
  for (js = excontext->j_subscribes; js != NULL; js = excontext->j_subscribes) {
    REMOVE_ELEMENT (excontext->j_subscribes, js);
    _eXosip_subscribe_free (excontext, js);
  }

  for (jn = excontext->j_notifies; jn != NULL; jn = excontext->j_notifies) {
    REMOVE_ELEMENT (excontext->j_notifies, jn);
    _eXosip_notify_free (excontext, jn);
  }
#endif

#ifndef OSIP_MONOTHREAD
  osip_mutex_destroy ((struct osip_mutex *) excontext->j_mutexlock);
#if !defined (_WIN32_WCE)
  osip_cond_destroy ((struct osip_cond *) excontext->j_cond);
#endif
#endif

  for (jreg = excontext->j_reg; jreg != NULL; jreg = excontext->j_reg) {
    REMOVE_ELEMENT (excontext->j_reg, jreg);
    _eXosip_reg_free (excontext, jreg);
  }

#ifndef MINISIZE
  for (jpub = excontext->j_pub; jpub != NULL; jpub = excontext->j_pub) {
    REMOVE_ELEMENT (excontext->j_pub, jpub);
    _eXosip_pub_free (excontext, jpub);
  }
#endif

  while (!osip_list_eol (&excontext->j_transactions, 0)) {
    osip_transaction_t *tr = (osip_transaction_t *) osip_list_get (&excontext->j_transactions, 0);

    if (tr->state == IST_TERMINATED || tr->state == ICT_TERMINATED || tr->state == NICT_TERMINATED || tr->state == NIST_TERMINATED) {
      OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_INFO1, NULL, "Release a terminated transaction\n"));
      osip_list_remove (&excontext->j_transactions, 0);
      _eXosip_delete_reserved (tr);
      _eXosip_dnsutils_release (tr->naptr_record);
      tr->naptr_record = NULL;
      osip_transaction_free (tr);
    }
    else {
      osip_list_remove (&excontext->j_transactions, 0);
      _eXosip_delete_reserved (tr);
      _eXosip_dnsutils_release (tr->naptr_record);
      tr->naptr_record = NULL;
      osip_transaction_free (tr);
    }
  }

  _eXosip_kill_transaction (&excontext->j_osip->osip_ict_transactions);
  _eXosip_kill_transaction (&excontext->j_osip->osip_nict_transactions);
  _eXosip_kill_transaction (&excontext->j_osip->osip_ist_transactions);
  _eXosip_kill_transaction (&excontext->j_osip->osip_nist_transactions);
  osip_release (excontext->j_osip);

  {
    eXosip_event_t *ev;

    for (ev = osip_fifo_tryget (excontext->j_events); ev != NULL; ev = osip_fifo_tryget (excontext->j_events))
      eXosip_event_free (ev);
  }

  osip_fifo_free (excontext->j_events);

  for (jauthinfo = excontext->authinfos; jauthinfo != NULL; jauthinfo = excontext->authinfos) {
    REMOVE_ELEMENT (excontext->authinfos, jauthinfo);
    osip_free (jauthinfo);
  }

  {
    struct eXosip_http_auth *http_auth;
    int pos;

    /* update entries with same call_id */
    for (pos = 0; pos < MAX_EXOSIP_HTTP_AUTH; pos++) {
      http_auth = &excontext->http_auths[pos];
      if (http_auth->pszCallId[0] == '\0')
        continue;
      osip_proxy_authenticate_free (http_auth->wa);
      memset (http_auth, 0, sizeof (struct eXosip_http_auth));
    }
  }

  if (excontext->eXtl_transport.tl_free != NULL)
    excontext->eXtl_transport.tl_free (excontext);

  memset (excontext, 0, sizeof (eXosip_t));
  excontext->j_stop_ua = -1;

  return;
}
