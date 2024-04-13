RenderViewTest::RenderViewTest() {
  RenderFrameImpl::InstallCreateHook(&TestRenderFrame::CreateTestRenderFrame);
}
