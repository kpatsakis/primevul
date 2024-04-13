std::string TestURLLoader::TestFailsBogusContentLength() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("/echo");
  request.SetMethod("POST");
  request.SetHeaders("Content-Length: 400");
  std::string postdata("postdata");
  request.AppendDataToBody(postdata.data(),
                           static_cast<uint32_t>(postdata.length()));

  int32_t rv;
  rv = OpenUntrusted(request, NULL);
  if (rv != PP_ERROR_NOACCESS)
    return ReportError(
        "Untrusted request with bogus Content-Length restriction", rv);

  PASS();
}
