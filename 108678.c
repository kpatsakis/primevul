ospf_packet_examin (struct ospf_header * oh, const unsigned bytesonwire)
{
  u_int16_t bytesdeclared;
  unsigned ret;
  struct ospf_ls_update * lsupd;

  /* Length, 1st approximation. */
  if (bytesonwire < OSPF_HEADER_SIZE)
  {
    if (IS_DEBUG_OSPF_PACKET (0, RECV))
      zlog_debug ("%s: undersized (%u B) packet", __func__, bytesonwire);
    return MSG_NG;
  }
  /* Now it is safe to access header fields. Performing length check, allow
   * for possible extra bytes of crypto auth/padding, which are not counted
   * in the OSPF header "length" field. */
  bytesdeclared = ntohs (oh->length);
  if (bytesdeclared > bytesonwire)
  {
    if (IS_DEBUG_OSPF_PACKET (0, RECV))
      zlog_debug ("%s: packet length error (%u real, %u declared)",
                  __func__, bytesonwire, bytesdeclared);
    return MSG_NG;
  }
  /* Length, 2nd approximation. The type-specific constraint is checked
     against declared length, not amount of bytes on wire. */
  if
  (
    oh->type >= OSPF_MSG_HELLO &&
    oh->type <= OSPF_MSG_LS_ACK &&
    bytesdeclared < OSPF_HEADER_SIZE + ospf_packet_minlen[oh->type]
  )
  {
    if (IS_DEBUG_OSPF_PACKET (0, RECV))
      zlog_debug ("%s: undersized (%u B) %s packet", __func__,
                  bytesdeclared, LOOKUP (ospf_packet_type_str, oh->type));
    return MSG_NG;
  }
  switch (oh->type)
  {
  case OSPF_MSG_HELLO:
    /* RFC2328 A.3.2, packet header + OSPF_HELLO_MIN_SIZE bytes followed
       by N>=0 router-IDs. */
    ret = (bytesonwire - OSPF_HEADER_SIZE - OSPF_HELLO_MIN_SIZE) % 4 ? MSG_NG : MSG_OK;
    break;
  case OSPF_MSG_DB_DESC:
    /* RFC2328 A.3.3, packet header + OSPF_DB_DESC_MIN_SIZE bytes followed
       by N>=0 header-only LSAs. */
    ret = ospf_lsaseq_examin
    (
      (struct lsa_header *) ((caddr_t) oh + OSPF_HEADER_SIZE + OSPF_DB_DESC_MIN_SIZE),
      bytesonwire - OSPF_HEADER_SIZE - OSPF_DB_DESC_MIN_SIZE,
      1, /* header-only LSAs */
      0
    );
    break;
  case OSPF_MSG_LS_REQ:
    /* RFC2328 A.3.4, packet header followed by N>=0 12-bytes request blocks. */
    ret = (bytesonwire - OSPF_HEADER_SIZE - OSPF_LS_REQ_MIN_SIZE) %
      OSPF_LSA_KEY_SIZE ? MSG_NG : MSG_OK;
    break;
  case OSPF_MSG_LS_UPD:
    /* RFC2328 A.3.5, packet header + OSPF_LS_UPD_MIN_SIZE bytes followed
       by N>=0 full LSAs (with N declared beforehand). */
    lsupd = (struct ospf_ls_update *) ((caddr_t) oh + OSPF_HEADER_SIZE);
    ret = ospf_lsaseq_examin
    (
      (struct lsa_header *) ((caddr_t) lsupd + OSPF_LS_UPD_MIN_SIZE),
      bytesonwire - OSPF_HEADER_SIZE - OSPF_LS_UPD_MIN_SIZE,
      0, /* full LSAs */
      ntohl (lsupd->num_lsas) /* 32 bits */
    );
    break;
  case OSPF_MSG_LS_ACK:
    /* RFC2328 A.3.6, packet header followed by N>=0 header-only LSAs. */
    ret = ospf_lsaseq_examin
    (
      (struct lsa_header *) ((caddr_t) oh + OSPF_HEADER_SIZE + OSPF_LS_ACK_MIN_SIZE),
      bytesonwire - OSPF_HEADER_SIZE - OSPF_LS_ACK_MIN_SIZE,
      1, /* header-only LSAs */
      0
    );
    break;
  default:
    if (IS_DEBUG_OSPF_PACKET (0, RECV))
      zlog_debug ("%s: invalid packet type 0x%02x", __func__, oh->type);
    return MSG_NG;
  }
  if (ret != MSG_OK && IS_DEBUG_OSPF_PACKET (0, RECV))
    zlog_debug ("%s: malformed %s packet", __func__, LOOKUP (ospf_packet_type_str, oh->type));
  return ret;
}