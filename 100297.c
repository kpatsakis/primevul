void InterstitialPage::InterstitialPageRVHViewDelegate::HandleMouseEvent() {
  if (interstitial_page_->tab() && interstitial_page_->tab()->GetViewDelegate())
    interstitial_page_->tab()->GetViewDelegate()->HandleMouseEvent();
}
