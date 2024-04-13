void InterstitialPage::SetSize(const gfx::Size& size) {
#if defined(OS_WIN) || defined(OS_LINUX)
  if (render_view_host_->view())
    render_view_host_->view()->SetSize(size);
#else
  NOTIMPLEMENTED();
#endif
}
