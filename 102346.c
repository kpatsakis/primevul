int XfrmController::addVirtualTunnelInterface(const std::string& deviceName,
 const std::string& localAddress,
 const std::string& remoteAddress, int32_t ikey,
 int32_t okey, bool isUpdate) {
    ALOGD("XfrmController::%s, line=%d", __FUNCTION__, __LINE__);
    ALOGD("deviceName=%s", deviceName.c_str());
    ALOGD("localAddress=%s", localAddress.c_str());
    ALOGD("remoteAddress=%s", remoteAddress.c_str());
    ALOGD("ikey=%0.8x", ikey);
    ALOGD("okey=%0.8x", okey);
    ALOGD("isUpdate=%d", isUpdate);

 if (deviceName.empty() || localAddress.empty() || remoteAddress.empty()) {
 return EINVAL;
 }

 const char* INFO_KIND_VTI6 = "vti6";
 const char* INFO_KIND_VTI = "vti";
 uint8_t PADDING_BUFFER[] = {0, 0, 0, 0};

 uint8_t remAddr[sizeof(in6_addr)];

 StatusOr<uint16_t> statusOrRemoteFam = convertStringAddress(remoteAddress, remAddr);
 if (!isOk(statusOrRemoteFam)) {
 return statusOrRemoteFam.status().code();
 }

 uint8_t locAddr[sizeof(in6_addr)];
 StatusOr<uint16_t> statusOrLocalFam = convertStringAddress(localAddress, locAddr);
 if (!isOk(statusOrLocalFam)) {
 return statusOrLocalFam.status().code();
 }

 if (statusOrLocalFam.value() != statusOrRemoteFam.value()) {
 return EINVAL;
 }

 uint16_t family = statusOrLocalFam.value();

    ifinfomsg ifInfoMsg{};

    nlattr iflaIfName;
 char iflaIfNameStrValue[deviceName.length() + 1];
 size_t iflaIfNameLength =
        strlcpy(iflaIfNameStrValue, deviceName.c_str(), sizeof(iflaIfNameStrValue));
 size_t iflaIfNamePad = fillNlAttr(IFLA_IFNAME, iflaIfNameLength, &iflaIfName);

 const std::string infoKindValue = (family == AF_INET6) ? INFO_KIND_VTI6 : INFO_KIND_VTI;
    nlattr iflaIfInfoKind;
 char infoKindValueStrValue[infoKindValue.length() + 1];
 size_t iflaIfInfoKindLength =
        strlcpy(infoKindValueStrValue, infoKindValue.c_str(), sizeof(infoKindValueStrValue));
 size_t iflaIfInfoKindPad = fillNlAttr(IFLA_INFO_KIND, iflaIfInfoKindLength, &iflaIfInfoKind);

    nlattr iflaVtiLocal;
 uint8_t binaryLocalAddress[sizeof(in6_addr)];
 size_t iflaVtiLocalPad =
        fillNlAttrIpAddress(IFLA_VTI_LOCAL, family, localAddress, &iflaVtiLocal,
                            netdutils::makeSlice(binaryLocalAddress));

    nlattr iflaVtiRemote;
 uint8_t binaryRemoteAddress[sizeof(in6_addr)];
 size_t iflaVtiRemotePad =
        fillNlAttrIpAddress(IFLA_VTI_REMOTE, family, remoteAddress, &iflaVtiRemote,
                            netdutils::makeSlice(binaryRemoteAddress));

    nlattr iflaVtiIKey;
 uint32_t iKeyValue;
 size_t iflaVtiIKeyPad = fillNlAttrU32(IFLA_VTI_IKEY, ikey, &iflaVtiIKey, &iKeyValue);

    nlattr iflaVtiOKey;
 uint32_t oKeyValue;
 size_t iflaVtiOKeyPad = fillNlAttrU32(IFLA_VTI_OKEY, okey, &iflaVtiOKey, &oKeyValue);

 int iflaInfoDataPayloadLength = iflaVtiLocal.nla_len + iflaVtiLocalPad + iflaVtiRemote.nla_len +
                                    iflaVtiRemotePad + iflaVtiIKey.nla_len + iflaVtiIKeyPad +
                                    iflaVtiOKey.nla_len + iflaVtiOKeyPad;

    nlattr iflaInfoData;
 size_t iflaInfoDataPad = fillNlAttr(IFLA_INFO_DATA, iflaInfoDataPayloadLength, &iflaInfoData);

    nlattr iflaLinkInfo;
 size_t iflaLinkInfoPad = fillNlAttr(IFLA_LINKINFO,
                                        iflaInfoData.nla_len + iflaInfoDataPad +
                                            iflaIfInfoKind.nla_len + iflaIfInfoKindPad,
 &iflaLinkInfo);

    iovec iov[] = {
 {NULL, 0},
 {&ifInfoMsg, sizeof(ifInfoMsg)},

 {&iflaIfName, sizeof(iflaIfName)},
 {iflaIfNameStrValue, iflaIfNameLength},
 {&PADDING_BUFFER, iflaIfNamePad},

 {&iflaLinkInfo, sizeof(iflaLinkInfo)},

 {&iflaIfInfoKind, sizeof(iflaIfInfoKind)},
 {infoKindValueStrValue, iflaIfInfoKindLength},
 {&PADDING_BUFFER, iflaIfInfoKindPad},

 {&iflaInfoData, sizeof(iflaInfoData)},

 {&iflaVtiLocal, sizeof(iflaVtiLocal)},
 {&binaryLocalAddress, (family == AF_INET) ? sizeof(in_addr) : sizeof(in6_addr)},
 {&PADDING_BUFFER, iflaVtiLocalPad},

 {&iflaVtiRemote, sizeof(iflaVtiRemote)},
 {&binaryRemoteAddress, (family == AF_INET) ? sizeof(in_addr) : sizeof(in6_addr)},
 {&PADDING_BUFFER, iflaVtiRemotePad},

 {&iflaVtiIKey, sizeof(iflaVtiIKey)},
 {&iKeyValue, sizeof(iKeyValue)},
 {&PADDING_BUFFER, iflaVtiIKeyPad},

 {&iflaVtiOKey, sizeof(iflaVtiOKey)},
 {&oKeyValue, sizeof(oKeyValue)},
 {&PADDING_BUFFER, iflaVtiOKeyPad},

 {&PADDING_BUFFER, iflaInfoDataPad},

 {&PADDING_BUFFER, iflaLinkInfoPad},
 };

 uint16_t action = RTM_NEWLINK;
 uint16_t flags = NLM_F_REQUEST | NLM_F_ACK;

 if (!isUpdate) {
        flags |= NLM_F_EXCL | NLM_F_CREATE;
 }

 int ret = -1 * sendNetlinkRequest(action, flags, iov, ARRAY_SIZE(iov), nullptr);
 if (ret) {
        ALOGE("Error in %s virtual tunnel interface. Error Code: %d",
              isUpdate ? "updating" : "adding", ret);
 }
 return ret;
}
