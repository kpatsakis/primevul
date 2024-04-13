void DevToolsWindow::UpdateFrontendDockSide() {
  base::StringValue dock_side(SideToString(dock_side_));
  CallClientFunction("InspectorFrontendAPI.setDockSide", &dock_side, NULL,
                     NULL);
  base::FundamentalValue docked(IsDocked());
  CallClientFunction("InspectorFrontendAPI.setAttachedWindow", &docked, NULL,
                     NULL);
}
