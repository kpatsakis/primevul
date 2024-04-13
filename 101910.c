void PaymentHandlerWebFlowViewController::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  if (navigation_handle->IsSameDocument())
    return;

  if (!OriginSecurityChecker::IsOriginSecure(navigation_handle->GetURL()) ||
      (!OriginSecurityChecker::IsSchemeCryptographic(
           navigation_handle->GetURL()) &&
       !OriginSecurityChecker::IsOriginLocalhostOrFile(
           navigation_handle->GetURL()) &&
       !navigation_handle->GetURL().IsAboutBlank()) ||
      !SslValidityChecker::IsSslCertificateValid(
          navigation_handle->GetWebContents())) {
    if (!net::IsLocalhost(navigation_handle->GetURL())) {
      log_.Error("Aborting payment handler window \"" + target_.spec() +
                 "\" because of navigation to an insecure url \"" +
                 navigation_handle->GetURL().spec() + "\"");
      AbortPayment();
      return;
    }
  }

  if (first_navigation_complete_callback_) {
    std::move(first_navigation_complete_callback_)
        .Run(true, web_contents()->GetMainFrame()->GetProcess()->GetID(),
             web_contents()->GetMainFrame()->GetRoutingID());
    first_navigation_complete_callback_ = PaymentHandlerOpenWindowCallback();
  }

  UpdateHeaderView();
}
