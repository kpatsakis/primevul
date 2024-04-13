std::string TestURLLoader::TestAuditURLRedirect() {
  pp::URLRequestInfo request(instance_);
  std::string redirect_prefix("/server-redirect?");
  std::string redirect_url =
      GetReachableAbsoluteURL("test_url_loader_data/hello.txt");
  request.SetURL(redirect_prefix.append(redirect_url));
  request.SetFollowRedirects(false);

  TestCompletionCallback callback(instance_->pp_instance(), callback_type());

  pp::URLLoader loader(instance_);
  callback.WaitForResult(loader.Open(request, callback.GetCallback()));
  CHECK_CALLBACK_BEHAVIOR(callback);
  ASSERT_EQ(PP_OK, callback.result());

  pp::URLResponseInfo response_info(loader.GetResponseInfo());
  if (response_info.is_null())
    return "URLLoader::GetResponseInfo returned null";
  int32_t status_code = response_info.GetStatusCode();
  if (status_code != 301)
    return "Response status should be 301";

  callback.WaitForResult(loader.FollowRedirect(callback.GetCallback()));
  CHECK_CALLBACK_BEHAVIOR(callback);
  ASSERT_EQ(PP_OK, callback.result());
  std::string body;
  std::string error = ReadEntireResponseBody(&loader, &body);
  if (!error.empty())
    return error;

  if (body != "hello\n")
    return "URLLoader::FollowRedirect failed";

  PASS();
}
