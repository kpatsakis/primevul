static void process_service_search_attr_rsp(tCONN_CB* p_ccb, uint8_t* p_reply,
 uint8_t* p_reply_end) {
 uint8_t *p, *p_start, *p_end, *p_param_len;
 uint8_t type;
 uint32_t seq_len;
 uint16_t param_len, lists_byte_count = 0;
 bool cont_request_needed = false;

#if (SDP_DEBUG_RAW == TRUE)
  SDP_TRACE_WARNING("process_service_search_attr_rsp");
#endif
 /* If p_reply is NULL, we were called for the initial read */
 if (p_reply) {
 if (p_reply + 4 /* transaction ID and length */ + sizeof(lists_byte_count) >
        p_reply_end) {
      android_errorWriteLog(0x534e4554, "79884292");
      sdp_disconnect(p_ccb, SDP_INVALID_PDU_SIZE);
 return;
 }

#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("ID & len: 0x%02x-%02x-%02x-%02x", p_reply[0], p_reply[1],
                      p_reply[2], p_reply[3]);
#endif
 /* Skip transaction ID and length */
    p_reply += 4;

    BE_STREAM_TO_UINT16(lists_byte_count, p_reply);
#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("lists_byte_count:%d", lists_byte_count);
#endif

 /* Copy the response to the scratchpad. First, a safety check on the length
     */
 if ((p_ccb->list_len + lists_byte_count) > SDP_MAX_LIST_BYTE_COUNT) {
      sdp_disconnect(p_ccb, SDP_INVALID_PDU_SIZE);
 return;
 }

#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("list_len: %d, list_byte_count: %d", p_ccb->list_len,
                      lists_byte_count);
#endif

 if (p_reply + lists_byte_count + 1 /* continuation */ > p_reply_end) {
      android_errorWriteLog(0x534e4554, "79884292");
      sdp_disconnect(p_ccb, SDP_INVALID_PDU_SIZE);
 return;
 }

 if (p_ccb->rsp_list == NULL)
      p_ccb->rsp_list = (uint8_t*)osi_malloc(SDP_MAX_LIST_BYTE_COUNT);
    memcpy(&p_ccb->rsp_list[p_ccb->list_len], p_reply, lists_byte_count);
    p_ccb->list_len += lists_byte_count;
    p_reply += lists_byte_count;
#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("list_len: %d(search_attr_rsp)", p_ccb->list_len);

 /* Check if we need to request a continuation */
    SDP_TRACE_WARNING("*p_reply:%d(%d)", *p_reply, SDP_MAX_CONTINUATION_LEN);
#endif
 if (*p_reply) {
 if (*p_reply > SDP_MAX_CONTINUATION_LEN) {
        sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
 return;
 }

      cont_request_needed = true;
 }
 }

#if (SDP_DEBUG_RAW == TRUE)
  SDP_TRACE_WARNING("cont_request_needed:%d", cont_request_needed);
#endif
 /* If continuation request (or first time request) */
 if ((cont_request_needed) || (!p_reply)) {
    BT_HDR* p_msg = (BT_HDR*)osi_malloc(SDP_DATA_BUF_SIZE);
 uint8_t* p;

    p_msg->offset = L2CAP_MIN_OFFSET;
    p = p_start = (uint8_t*)(p_msg + 1) + L2CAP_MIN_OFFSET;

 /* Build a service search request packet */
    UINT8_TO_BE_STREAM(p, SDP_PDU_SERVICE_SEARCH_ATTR_REQ);
    UINT16_TO_BE_STREAM(p, p_ccb->transaction_id);
    p_ccb->transaction_id++;

 /* Skip the length, we need to add it at the end */
    p_param_len = p;
    p += 2;

/* Build the UID sequence. */
#if (SDP_BROWSE_PLUS == TRUE)
    p = sdpu_build_uuid_seq(p, 1,
 &p_ccb->p_db->uuid_filters[p_ccb->cur_uuid_idx]);
#else
    p = sdpu_build_uuid_seq(p, p_ccb->p_db->num_uuid_filters,
                            p_ccb->p_db->uuid_filters);
#endif

 /* Max attribute byte count */
    UINT16_TO_BE_STREAM(p, sdp_cb.max_attr_list_size);

 /* If no attribute filters, build a wildcard attribute sequence */
 if (p_ccb->p_db->num_attr_filters)
      p = sdpu_build_attrib_seq(p, p_ccb->p_db->attr_filters,
                                p_ccb->p_db->num_attr_filters);
 else
      p = sdpu_build_attrib_seq(p, NULL, 0);

 /* No continuation for first request */
 if (p_reply) {
 if ((p_reply + *p_reply + 1) <= p_reply_end) {
        memcpy(p, p_reply, *p_reply + 1);
        p += *p_reply + 1;
 } else {
        android_errorWriteLog(0x534e4554, "68161546");
 }
 } else
      UINT8_TO_BE_STREAM(p, 0);

 /* Go back and put the parameter length into the buffer */
    param_len = p - p_param_len - 2;
    UINT16_TO_BE_STREAM(p_param_len, param_len);

 /* Set the length of the SDP data in the buffer */
    p_msg->len = p - p_start;

    L2CA_DataWrite(p_ccb->connection_id, p_msg);

 /* Start inactivity timer */
    alarm_set_on_mloop(p_ccb->sdp_conn_timer, SDP_INACT_TIMEOUT_MS,
                       sdp_conn_timer_timeout, p_ccb);

 return;
 }

/*******************************************************************/
/* We now have the full response, which is a sequence of sequences */
/*******************************************************************/

#if (SDP_RAW_DATA_INCLUDED == TRUE)
  SDP_TRACE_WARNING("process_service_search_attr_rsp");
  sdp_copy_raw_data(p_ccb, true);
#endif

  p = &p_ccb->rsp_list[0];

 /* The contents is a sequence of attribute sequences */
  type = *p++;

 if ((type >> 3) != DATA_ELE_SEQ_DESC_TYPE) {
    SDP_TRACE_WARNING("SDP - Wrong type: 0x%02x in attr_rsp", type);
 return;
 }
  p = sdpu_get_len_from_type(p, type, &seq_len);

  p_end = &p_ccb->rsp_list[p_ccb->list_len];

 if ((p + seq_len) != p_end) {
    sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
 return;
 }

 while (p < p_end) {
    p = save_attr_seq(p_ccb, p, &p_ccb->rsp_list[p_ccb->list_len]);
 if (!p) {
      sdp_disconnect(p_ccb, SDP_DB_FULL);
 return;
 }
 }

 /* Since we got everything we need, disconnect the call */
  sdp_disconnect(p_ccb, SDP_SUCCESS);
}
