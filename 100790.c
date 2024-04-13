  void AddAuthToCache() {
    const base::string16 kFoo(ASCIIToUTF16("foo"));
    const base::string16 kBar(ASCIIToUTF16("bar"));
    GURL proxy_url(GetParam() == HTTP ? "http://proxy" : "https://proxy:80");
    session_->http_auth_cache()->Add(proxy_url,
                                     "MyRealm1",
                                     HttpAuth::AUTH_SCHEME_BASIC,
                                     "Basic realm=MyRealm1",
                                     AuthCredentials(kFoo, kBar),
                                     "/");
  }
