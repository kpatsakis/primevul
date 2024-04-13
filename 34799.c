eXosip_masquerade_contact (struct eXosip_t *excontext, const char *public_address, int port)
{
  if (excontext->eXtl_transport.tl_masquerade_contact == NULL) {
    OSIP_TRACE (osip_trace (__FILE__, __LINE__, OSIP_WARNING, NULL, "no transport protocol selected!\n"));
    if (public_address == NULL || public_address[0] == '\0') {
      memset (excontext->udp_firewall_ip, '\0', sizeof (excontext->udp_firewall_ip));
      memset (excontext->udp_firewall_port, '\0', sizeof (excontext->udp_firewall_port));
      memset (excontext->tcp_firewall_ip, '\0', sizeof (excontext->tcp_firewall_ip));
      memset (excontext->tcp_firewall_port, '\0', sizeof (excontext->tcp_firewall_port));
      memset (excontext->tls_firewall_ip, '\0', sizeof (excontext->tls_firewall_ip));
      memset (excontext->tls_firewall_port, '\0', sizeof (excontext->tls_firewall_port));
      memset (excontext->dtls_firewall_ip, '\0', sizeof (excontext->dtls_firewall_ip));
      memset (excontext->dtls_firewall_port, '\0', sizeof (excontext->dtls_firewall_port));
      return;
    }
    snprintf (excontext->udp_firewall_ip, sizeof (excontext->udp_firewall_ip), "%s", public_address);
    snprintf (excontext->tcp_firewall_ip, sizeof (excontext->tcp_firewall_ip), "%s", public_address);
    snprintf (excontext->tls_firewall_ip, sizeof (excontext->tls_firewall_ip), "%s", public_address);
    snprintf (excontext->dtls_firewall_ip, sizeof (excontext->dtls_firewall_ip), "%s", public_address);
    if (port > 0) {
      snprintf (excontext->udp_firewall_port, sizeof (excontext->udp_firewall_port), "%i", port);
      snprintf (excontext->tcp_firewall_port, sizeof (excontext->tcp_firewall_port), "%i", port);
      snprintf (excontext->tls_firewall_port, sizeof (excontext->tls_firewall_port), "%i", port);
      snprintf (excontext->dtls_firewall_port, sizeof (excontext->dtls_firewall_port), "%i", port);
    }
    return;
  }
  excontext->eXtl_transport.tl_masquerade_contact (excontext, public_address, port);
  return;
}
