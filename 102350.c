netdutils::Status XfrmController::flushPolicyDb(const XfrmSocket& s) {
    std::vector<iovec> iov = {{NULL, 0}}; // reserved for the eventual addition of a NLMSG_HDR
 return s.sendMessage(XFRM_MSG_FLUSHPOLICY, NETLINK_REQUEST_FLAGS, 0, &iov);
}
