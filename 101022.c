void Instance::CreateVerticalScrollbar() {
  if (v_scrollbar_.get())
    return;

  v_scrollbar_.reset(new pp::Scrollbar_Dev(this, true));
}
