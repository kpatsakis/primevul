void RenderView::CapturePageInfo(int load_id, bool preliminary_capture) {
  if (load_id != page_id_)
    return;  // this capture call is no longer relevant due to navigation
  if (load_id == last_indexed_page_id_)
    return;  // we already indexed this page

  if (!webview())
    return;

  WebFrame* main_frame = webview()->mainFrame();
  if (!main_frame)
    return;

  if (main_frame->isViewSourceModeEnabled())
    return;

  WebDataSource* ds = main_frame->dataSource();
  if (ds && ds->hasUnreachableURL())
    return;

  if (!preliminary_capture)
    last_indexed_page_id_ = load_id;

  GURL url(main_frame->url());
  if (url.is_empty())
    return;

  string16 contents;
  CaptureText(main_frame, &contents);
  if (contents.size()) {
    base::TimeTicks begin_time = base::TimeTicks::Now();
    std::string language = DetermineTextLanguage(contents);
    UMA_HISTOGRAM_MEDIUM_TIMES("Renderer4.LanguageDetection",
                               base::TimeTicks::Now() - begin_time);
    WebKit::WebDocument document = main_frame->document();
    Send(new ViewHostMsg_PageContents(routing_id_, url, load_id, contents,
                                      language, IsPageTranslatable(&document)));
  }

  OnCaptureThumbnail();
}
