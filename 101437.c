int GetCertID(net::URLRequest* request, int child_id) {
  if (request->ssl_info().cert.get()) {
    return CertStore::GetInstance()->StoreCert(request->ssl_info().cert.get(),
                                               child_id);
  }
  return 0;
}
