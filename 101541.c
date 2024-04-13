void OmniboxViewViews::AddedToWidget() {
  views::Textfield::AddedToWidget();
  scoped_compositor_observer_.Add(GetWidget()->GetCompositor());
}
