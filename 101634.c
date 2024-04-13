  gfx::Rect GetBoundingClientRect(FrameTreeNode* node, const std::string& sel) {
    std::string result;
    EXPECT_TRUE(ExecuteScriptAndExtractString(
        node,
        base::StringPrintf(
            "window.domAutomationController.send(rectAsString("
            "    document.querySelector('%s').getBoundingClientRect()));",
            sel.c_str()),
        &result));
    return GetRectFromString(result);
  }
