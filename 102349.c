netdutils::Status XfrmController::fillXfrmId(const std::string& sourceAddress,
 const std::string& destinationAddress, int32_t spi,
 int32_t markValue, int32_t markMask,
 int32_t transformId, XfrmId* xfrmId) {
    xfrmId->transformId = transformId;
    xfrmId->spi = htonl(spi);
    xfrmId->mark.v = markValue;
    xfrmId->mark.m = markMask;

 xfrm_address_t sourceXfrmAddr{}, destXfrmAddr{};
 StatusOr<int> sourceFamily, destFamily;
    sourceFamily = convertToXfrmAddr(sourceAddress, &sourceXfrmAddr);
    destFamily = convertToXfrmAddr(destinationAddress, &destXfrmAddr);
 if (!isOk(sourceFamily) || !isOk(destFamily)) {
 return netdutils::statusFromErrno(EINVAL, "Invalid address " + sourceAddress + "/" +
                                                      destinationAddress);
 }

 if (destFamily.value() == AF_UNSPEC ||
 (sourceFamily.value() != AF_UNSPEC && sourceFamily.value() != destFamily.value())) {
        ALOGD("Invalid or Mismatched Address Families, %d != %d, line=%d", sourceFamily.value(),
              destFamily.value(), __LINE__);
 return netdutils::statusFromErrno(EINVAL, "Invalid or mismatched address families");
 }

    xfrmId->addrFamily = destFamily.value();

    xfrmId->dstAddr = destXfrmAddr;
    xfrmId->srcAddr = sourceXfrmAddr;
 return netdutils::status::ok;
}
