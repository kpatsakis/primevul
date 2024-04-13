void rfc_send_rpn(tRFC_MCB* p_mcb, uint8_t dlci, bool is_command,
                  tPORT_STATE* p_pars, uint16_t mask) {
 uint8_t* p_data;
  BT_HDR* p_buf = (BT_HDR*)osi_malloc(RFCOMM_CMD_BUF_SIZE);

  p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
  p_data = (uint8_t*)(p_buf + 1) + p_buf->offset;

 *p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_RPN;

 if (!p_pars) {
 *p_data++ = RFCOMM_EA | (RFCOMM_MX_RPN_REQ_LEN << 1);

 *p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);

    p_buf->len = RFCOMM_MX_RPN_REQ_LEN + 2;
 } else {
 *p_data++ = RFCOMM_EA | (RFCOMM_MX_RPN_LEN << 1);

 *p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);
 *p_data++ = p_pars->baud_rate;
 *p_data++ = (p_pars->byte_size << RFCOMM_RPN_BITS_SHIFT) |
 (p_pars->stop_bits << RFCOMM_RPN_STOP_BITS_SHIFT) |
 (p_pars->parity << RFCOMM_RPN_PARITY_SHIFT) |
 (p_pars->parity_type << RFCOMM_RPN_PARITY_TYPE_SHIFT);
 *p_data++ = p_pars->fc_type;
 *p_data++ = p_pars->xon_char;
 *p_data++ = p_pars->xoff_char;
 *p_data++ = (mask & 0xFF);
 *p_data++ = (mask >> 8);

 /* Total length is sizeof RPN data + mx header 2 */
    p_buf->len = RFCOMM_MX_RPN_LEN + 2;
 }

  rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}
