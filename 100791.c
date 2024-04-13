  HttpNetworkSession* CreateNetworkSession() {
    return SpdySessionDependencies::SpdyCreateSessionDeterministic(
        &session_deps_);
  }
