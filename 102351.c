netdutils::Status XfrmController::ipSecDeleteSecurityPolicy(int32_t transformId, int32_t direction,
 const std::string& localAddress,
 const std::string& remoteAddress,
 int32_t markValue, int32_t markMask) {
 return processSecurityPolicy(transformId, direction, localAddress, remoteAddress, 0, markValue,
                                 markMask, XFRM_MSG_DELPOLICY);
}
