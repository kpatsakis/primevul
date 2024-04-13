std::string TestURLLoader::TestBasicPOST() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("/echo");
  request.SetMethod("POST");
  std::string postdata("postdata");
  request.AppendDataToBody(postdata.data(),
                           static_cast<uint32_t>(postdata.length()));
  return LoadAndCompareBody(request, postdata);
}
