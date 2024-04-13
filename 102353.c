netdutils::Status XfrmController::updateTunnelModeSecurityPolicy(const XfrmSaInfo& record,
 const XfrmSocket& sock,
 XfrmDirection direction,
 uint16_t msgType) {
    xfrm_userpolicy_info userpolicy{};
    nlattr_user_tmpl usertmpl{};
    nlattr_xfrm_mark xfrmmark{};

 enum {
        NLMSG_HDR,
        USERPOLICY,
        USERPOLICY_PAD,
        USERTMPL,
        USERTMPL_PAD,
        MARK,
        MARK_PAD,
 };

    std::vector<iovec> iov = {
 {NULL, 0}, // reserved for the eventual addition of a NLMSG_HDR
 {&userpolicy, 0}, // main xfrm_userpolicy_info struct
 {kPadBytes, 0}, // up to NLMSG_ALIGNTO pad bytes of padding
 {&usertmpl, 0}, // adjust size if xfrm_user_tmpl struct is present
 {kPadBytes, 0}, // up to NLATTR_ALIGNTO pad bytes
 {&xfrmmark, 0}, // adjust size if xfrm mark is present
 {kPadBytes, 0}, // up to NLATTR_ALIGNTO pad bytes
 };

 int len;
    len = iov[USERPOLICY].iov_len = fillTransportModeUserSpInfo(record, direction, &userpolicy);
    iov[USERPOLICY_PAD].iov_len = NLMSG_ALIGN(len) - len;

    len = iov[USERTMPL].iov_len = fillNlAttrUserTemplate(record, &usertmpl);
    iov[USERTMPL_PAD].iov_len = NLA_ALIGN(len) - len;

    len = iov[MARK].iov_len = fillNlAttrXfrmMark(record, &xfrmmark);
    iov[MARK_PAD].iov_len = NLA_ALIGN(len) - len;

 return sock.sendMessage(msgType, NETLINK_REQUEST_FLAGS, 0, &iov);
}
