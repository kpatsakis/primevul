WebURLLoaderImpl::Context::Context(WebURLLoaderImpl* loader)
    : loader_(loader),
      client_(NULL),
      referrer_policy_(WebKit::WebReferrerPolicyDefault) {
}
