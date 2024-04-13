ospf_packet_examin (struct ospf_header * oh, const unsigned bytesonwire)
{
  u_int16_t bytesdeclared;

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
  return MSG_OK;
}