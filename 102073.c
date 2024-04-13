std::string TestURLLoader::TestCompoundBodyPOST() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("/echo");
  request.SetMethod("POST");
  std::string postdata1("post");
  request.AppendDataToBody(postdata1.data(),
                           static_cast<uint32_t>(postdata1.length()));
  std::string postdata2("data");
  request.AppendDataToBody(postdata2.data(),
                           static_cast<uint32_t>(postdata2.length()));
  return LoadAndCompareBody(request, postdata1 + postdata2);
}
