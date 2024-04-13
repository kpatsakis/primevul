void OpenUrlViaClickTarget(const ToRenderFrameHost& adapter, const GURL& url) {
  EXPECT_TRUE(ExecuteScript(adapter,
      std::string(kOpenUrlViaClickTargetFunc) + "(\"" + url.spec() + "\");"));
}
