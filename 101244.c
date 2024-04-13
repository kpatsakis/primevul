void OutOfProcessInstance::LoadUrlInternal(
    const std::string& url,
    pp::URLLoader* loader,
    void (OutOfProcessInstance::* method)(int32_t)) {
  pp::URLRequestInfo request(this);
  request.SetURL(url);
  request.SetMethod("GET");

  *loader = CreateURLLoaderInternal();
  pp::CompletionCallback callback = loader_factory_.NewCallback(method);
  int rv = loader->Open(request, callback);
  if (rv != PP_OK_COMPLETIONPENDING)
    callback.Run(rv);
}
