void NavigateNamedFrame(const ToRenderFrameHost& caller_frame,
                        const GURL& url,
                        const std::string& name) {
  bool success = false;
  EXPECT_TRUE(ExecuteScriptAndExtractBool(
      caller_frame,
      "window.domAutomationController.send("
      "    !!window.open('" + url.spec() + "', '" + name + "'));",
      &success));
  EXPECT_TRUE(success);
}
