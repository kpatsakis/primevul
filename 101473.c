void V4L2JpegEncodeAccelerator::EncodedInstance::PrepareJpegMarkers(
    gfx::Size coded_size) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());
  const int kNumDQT = 2;
  for (size_t i = 0; i < kNumDQT; ++i) {
    const uint8_t kQuantSegment[] = {
        0xFF, JPEG_DQT, 0x00,
        0x03 + kDctSize,         // Segment length:67 (2-byte).
        static_cast<uint8_t>(i)  // Precision (4-bit high) = 0,
    };
    for (size_t j = 0; j < sizeof(kQuantSegment); ++j) {
      jpeg_markers_.push_back(kQuantSegment[j]);
    }

    for (size_t j = 0; j < kDctSize; ++j) {
      jpeg_markers_.push_back(quantization_table_[i].value[j]);
    }
  }

  const int kNumOfComponents = 3;
  const uint8_t kStartOfFrame[] = {
      0xFF,
      JPEG_SOF0,  // Baseline.
      0x00,
      0x11,  // Segment length:17 (2-byte).
      8,     // Data precision.
      static_cast<uint8_t>((coded_size.height() >> 8) & 0xFF),
      static_cast<uint8_t>(coded_size.height() & 0xFF),
      static_cast<uint8_t>((coded_size.width() >> 8) & 0xFF),
      static_cast<uint8_t>(coded_size.width() & 0xFF),
      kNumOfComponents,
  };
  for (size_t i = 0; i < sizeof(kStartOfFrame); ++i) {
    jpeg_markers_.push_back(kStartOfFrame[i]);
  }
  for (size_t i = 0; i < kNumOfComponents; ++i) {
    uint8_t h_sample_factor = 1;
    uint8_t v_sample_factor = 1;
    uint8_t quant_table_number = 1;
    if (!i) {
      h_sample_factor = 2;
      v_sample_factor = 2;
      quant_table_number = 0;
    }

    jpeg_markers_.push_back(i + 1);
    jpeg_markers_.push_back((h_sample_factor << 4) | v_sample_factor);
    jpeg_markers_.push_back(quant_table_number);
  }

  static const uint8_t kDcSegment[] = {
      0xFF, JPEG_DHT, 0x00,
      0x1F,  // Segment length:31 (2-byte).
  };
  static const uint8_t kAcSegment[] = {
      0xFF, JPEG_DHT, 0x00,
      0xB5,  // Segment length:181 (2-byte).
  };

  const int kNumHuffmanTables = 2;
  for (size_t i = 0; i < kNumHuffmanTables; ++i) {
    for (size_t j = 0; j < sizeof(kDcSegment); ++j) {
      jpeg_markers_.push_back(kDcSegment[j]);
    }

    jpeg_markers_.push_back(static_cast<uint8_t>(i));

    const JpegHuffmanTable& dcTable = kDefaultDcTable[i];
    for (size_t j = 0; j < kNumDcRunSizeBits; ++j)
      jpeg_markers_.push_back(dcTable.code_length[j]);
    for (size_t j = 0; j < kNumDcCodeWordsHuffVal; ++j)
      jpeg_markers_.push_back(dcTable.code_value[j]);

    for (size_t j = 0; j < sizeof(kAcSegment); ++j) {
      jpeg_markers_.push_back(kAcSegment[j]);
    }

    jpeg_markers_.push_back(0x10 | static_cast<uint8_t>(i));

    const JpegHuffmanTable& acTable = kDefaultAcTable[i];
    for (size_t j = 0; j < kNumAcRunSizeBits; ++j)
      jpeg_markers_.push_back(acTable.code_length[j]);
    for (size_t j = 0; j < kNumAcCodeWordsHuffVal; ++j)
      jpeg_markers_.push_back(acTable.code_value[j]);
  }

  static const uint8_t kStartOfScan[] = {
      0xFF, JPEG_SOS, 0x00,
      0x0C,  // Segment Length:12 (2-byte).
      0x03   // Number of components in scan.
  };
  for (size_t i = 0; i < sizeof(kStartOfScan); ++i) {
    jpeg_markers_.push_back(kStartOfScan[i]);
  }

  for (uint8_t i = 0; i < kNumOfComponents; ++i) {
    uint8_t dc_table_number = 1;
    uint8_t ac_table_number = 1;
    if (!i) {
      dc_table_number = 0;
      ac_table_number = 0;
    }

    jpeg_markers_.push_back(i + 1);
    jpeg_markers_.push_back((dc_table_number << 4) | ac_table_number);
  }
  jpeg_markers_.push_back(0x00);  // 0 for Baseline.
  jpeg_markers_.push_back(0x3F);  // 63 for Baseline.
  jpeg_markers_.push_back(0x00);  // 0 for Baseline.
}
