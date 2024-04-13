void WaitForChildFrameSurfaceReady(content::RenderFrameHost* child_frame) {
  RenderWidgetHostViewBase* child_view =
      static_cast<RenderFrameHostImpl*>(child_frame)
          ->GetRenderWidgetHost()
          ->GetView();
  if (!child_view || !child_view->IsRenderWidgetHostViewChildFrame())
    return;

  RenderWidgetHostViewBase* root_view =
      static_cast<CrossProcessFrameConnector*>(
          static_cast<RenderWidgetHostViewChildFrame*>(child_view)
              ->FrameConnectorForTesting())
          ->GetRootRenderWidgetHostViewForTesting();

  SurfaceHitTestReadyNotifier notifier(child_view);
  notifier.WaitForSurfaceReady(root_view);
}
