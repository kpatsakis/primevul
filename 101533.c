bool PDFiumEngine::MouseDownState::Matches(
    const PDFiumPage::Area& area,
    const PDFiumPage::LinkTarget& target) const {
  if (area_ != area)
    return false;

  if (area == PDFiumPage::WEBLINK_AREA)
    return target_.url == target.url;

  if (area == PDFiumPage::DOCLINK_AREA)
    return target_.page == target.page;

  return true;
}
