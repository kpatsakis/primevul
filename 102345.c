netdutils::Status XfrmController::Init() {
    RETURN_IF_NOT_OK(flushInterfaces());
 XfrmSocketImpl sock;
    RETURN_IF_NOT_OK(sock.open());
    RETURN_IF_NOT_OK(flushSaDb(sock));
 return flushPolicyDb(sock);
}
