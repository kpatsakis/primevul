static uint8_t* sdpu_build_uuid_seq(uint8_t* p_out, uint16_t num_uuids,
 Uuid* p_uuid_list) {
 uint16_t xx;
 uint8_t* p_len;

 /* First thing is the data element header */
  UINT8_TO_BE_STREAM(p_out, (DATA_ELE_SEQ_DESC_TYPE << 3) | SIZE_IN_NEXT_BYTE);

 /* Remember where the length goes. Leave space for it. */
  p_len = p_out;
  p_out += 1;

 /* Now, loop through and put in all the UUID(s) */
 for (xx = 0; xx < num_uuids; xx++, p_uuid_list++) {
 int len = p_uuid_list->GetShortestRepresentationSize();
 if (len == Uuid::kNumBytes16) {
      UINT8_TO_BE_STREAM(p_out, (UUID_DESC_TYPE << 3) | SIZE_TWO_BYTES);
      UINT16_TO_BE_STREAM(p_out, p_uuid_list->As16Bit());
 } else if (len == Uuid::kNumBytes32) {
      UINT8_TO_BE_STREAM(p_out, (UUID_DESC_TYPE << 3) | SIZE_FOUR_BYTES);
      UINT32_TO_BE_STREAM(p_out, p_uuid_list->As32Bit());
 } else if (len == Uuid::kNumBytes128) {
      UINT8_TO_BE_STREAM(p_out, (UUID_DESC_TYPE << 3) | SIZE_SIXTEEN_BYTES);
      ARRAY_TO_BE_STREAM(p_out, p_uuid_list->To128BitBE(),
 (int)Uuid::kNumBytes128);
 } else {
      DCHECK(0) << "SDP: Passed UUID has invalid length " << len;
 }
 }

 /* Now, put in the length */
  xx = (uint16_t)(p_out - p_len - 1);
  UINT8_TO_BE_STREAM(p_len, xx);

 return (p_out);
}
