status_t HevcParameterSets::parseVps(const uint8_t* data, size_t size) {
 NALBitReader reader(data, size);
    reader.skipBits(4);
    reader.skipBits(1);
    reader.skipBits(1);
    reader.skipBits(6);
    reader.skipBits(1);
    reader.skipBits(16);

 if (reader.atLeastNumBitsLeft(96)) {
        mParams.add(kGeneralProfileSpace, reader.getBits(2));
        mParams.add(kGeneralTierFlag, reader.getBits(1));
        mParams.add(kGeneralProfileIdc, reader.getBits(5));
        mParams.add(kGeneralProfileCompatibilityFlags, reader.getBits(32));
        mParams.add(
                kGeneralConstraintIndicatorFlags,
 ((uint64_t)reader.getBits(16) << 32) | reader.getBits(32));
        mParams.add(kGeneralLevelIdc, reader.getBits(8));
 } else {
        reader.skipBits(96);
 }

 return reader.overRead() ? ERROR_MALFORMED : OK;
}
