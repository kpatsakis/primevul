void RenderView::showSpellingUI(bool show) {
  Send(new ViewHostMsg_ShowSpellingPanel(routing_id_, show));
}
