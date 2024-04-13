StatusOr<int> convertToXfrmAddr(const std::string& strAddr, xfrm_address_t* xfrmAddr) {
 if (strAddr.length() == 0) {
        memset(xfrmAddr, 0, sizeof(*xfrmAddr));
 return AF_UNSPEC;
 }

 if (inet_pton(AF_INET6, strAddr.c_str(), reinterpret_cast<void*>(xfrmAddr))) {
 return AF_INET6;
 } else if (inet_pton(AF_INET, strAddr.c_str(), reinterpret_cast<void*>(xfrmAddr))) {
 return AF_INET;
 } else {
 return netdutils::statusFromErrno(EAFNOSUPPORT, "Invalid address family");
 }
}
