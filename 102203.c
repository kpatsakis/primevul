status_t ATSParser::Stream::parsePES(ABitReader *br, SyncEvent *event) {
 unsigned packet_startcode_prefix = br->getBits(24);

    ALOGV("packet_startcode_prefix = 0x%08x", packet_startcode_prefix);

 if (packet_startcode_prefix != 1) {
        ALOGV("Supposedly payload_unit_start=1 unit does not start "
 "with startcode.");

 return ERROR_MALFORMED;
 }

 unsigned stream_id = br->getBits(8);
    ALOGV("stream_id = 0x%02x", stream_id);

 unsigned PES_packet_length = br->getBits(16);
    ALOGV("PES_packet_length = %u", PES_packet_length);

 if (stream_id != 0xbc // program_stream_map
 && stream_id != 0xbe // padding_stream
 && stream_id != 0xbf // private_stream_2
 && stream_id != 0xf0 // ECM
 && stream_id != 0xf1 // EMM
 && stream_id != 0xff // program_stream_directory
 && stream_id != 0xf2 // DSMCC
 && stream_id != 0xf8) { // H.222.1 type E
 if (br->getBits(2) != 2u) {
 return ERROR_MALFORMED;
 }

        MY_LOGV("PES_scrambling_control = %u", br->getBits(2));
        MY_LOGV("PES_priority = %u", br->getBits(1));
        MY_LOGV("data_alignment_indicator = %u", br->getBits(1));
        MY_LOGV("copyright = %u", br->getBits(1));
        MY_LOGV("original_or_copy = %u", br->getBits(1));

 unsigned PTS_DTS_flags = br->getBits(2);
        ALOGV("PTS_DTS_flags = %u", PTS_DTS_flags);

 unsigned ESCR_flag = br->getBits(1);
        ALOGV("ESCR_flag = %u", ESCR_flag);

 unsigned ES_rate_flag = br->getBits(1);
        ALOGV("ES_rate_flag = %u", ES_rate_flag);

 unsigned DSM_trick_mode_flag = br->getBits(1);
        ALOGV("DSM_trick_mode_flag = %u", DSM_trick_mode_flag);

 unsigned additional_copy_info_flag = br->getBits(1);
        ALOGV("additional_copy_info_flag = %u", additional_copy_info_flag);

        MY_LOGV("PES_CRC_flag = %u", br->getBits(1));
        MY_LOGV("PES_extension_flag = %u", br->getBits(1));

 unsigned PES_header_data_length = br->getBits(8);
        ALOGV("PES_header_data_length = %u", PES_header_data_length);

 unsigned optional_bytes_remaining = PES_header_data_length;

 uint64_t PTS = 0, DTS = 0;

 if (PTS_DTS_flags == 2 || PTS_DTS_flags == 3) {
 if (optional_bytes_remaining < 5u) {
 return ERROR_MALFORMED;
 }

 if (br->getBits(4) != PTS_DTS_flags) {
 return ERROR_MALFORMED;
 }
            PTS = ((uint64_t)br->getBits(3)) << 30;
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }
            PTS |= ((uint64_t)br->getBits(15)) << 15;
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }
            PTS |= br->getBits(15);
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }

            ALOGV("PTS = 0x%016" PRIx64 " (%.2f)", PTS, PTS / 90000.0);

            optional_bytes_remaining -= 5;

 if (PTS_DTS_flags == 3) {
 if (optional_bytes_remaining < 5u) {
 return ERROR_MALFORMED;
 }

 if (br->getBits(4) != 1u) {
 return ERROR_MALFORMED;
 }

                DTS = ((uint64_t)br->getBits(3)) << 30;
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }
                DTS |= ((uint64_t)br->getBits(15)) << 15;
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }
                DTS |= br->getBits(15);
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }

                ALOGV("DTS = %" PRIu64, DTS);

                optional_bytes_remaining -= 5;
 }
 }

 if (ESCR_flag) {
 if (optional_bytes_remaining < 6u) {
 return ERROR_MALFORMED;
 }

            br->getBits(2);

 uint64_t ESCR = ((uint64_t)br->getBits(3)) << 30;
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }
            ESCR |= ((uint64_t)br->getBits(15)) << 15;
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }
            ESCR |= br->getBits(15);
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }

            ALOGV("ESCR = %" PRIu64, ESCR);
            MY_LOGV("ESCR_extension = %u", br->getBits(9));

 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }

            optional_bytes_remaining -= 6;
 }

 if (ES_rate_flag) {
 if (optional_bytes_remaining < 3u) {
 return ERROR_MALFORMED;
 }

 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }
            MY_LOGV("ES_rate = %u", br->getBits(22));
 if (br->getBits(1) != 1u) {
 return ERROR_MALFORMED;
 }

            optional_bytes_remaining -= 3;
 }

        br->skipBits(optional_bytes_remaining * 8);


 if (PES_packet_length != 0) {
 if (PES_packet_length < PES_header_data_length + 3) {
 return ERROR_MALFORMED;
 }

 unsigned dataLength =
                PES_packet_length - 3 - PES_header_data_length;

 if (br->numBitsLeft() < dataLength * 8) {
                ALOGE("PES packet does not carry enough data to contain "
 "payload. (numBitsLeft = %zu, required = %u)",
                     br->numBitsLeft(), dataLength * 8);

 return ERROR_MALFORMED;
 }

            onPayloadData(
                    PTS_DTS_flags, PTS, DTS, br->data(), dataLength, event);

            br->skipBits(dataLength * 8);
 } else {
            onPayloadData(
                    PTS_DTS_flags, PTS, DTS,
                    br->data(), br->numBitsLeft() / 8, event);

 size_t payloadSizeBits = br->numBitsLeft();
 if (payloadSizeBits % 8 != 0u) {
 return ERROR_MALFORMED;
 }

            ALOGV("There's %zu bytes of payload.", payloadSizeBits / 8);
 }
 } else if (stream_id == 0xbe) { // padding_stream
 if (PES_packet_length == 0u) {
 return ERROR_MALFORMED;
 }
        br->skipBits(PES_packet_length * 8);
 } else {
 if (PES_packet_length == 0u) {
 return ERROR_MALFORMED;
 }
        br->skipBits(PES_packet_length * 8);
 }

 return OK;
}
