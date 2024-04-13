void Document::Initialize() {
  DCHECK_EQ(lifecycle_.GetState(), DocumentLifecycle::kInactive);
  DCHECK(!ax_object_cache_ || this != &AXObjectCacheOwner());

  layout_view_ = new LayoutView(this);
  SetLayoutObject(layout_view_);

  layout_view_->SetIsInWindow(true);
  layout_view_->SetStyle(StyleResolver::StyleForViewport(*this));
  layout_view_->Compositor()->SetNeedsCompositingUpdate(
      kCompositingUpdateAfterCompositingInputChange);

  {
    ReattachLegacyLayoutObjectList legacy_layout_objects(*this);
    AttachContext context;
    ContainerNode::AttachLayoutTree(context);
    legacy_layout_objects.ForceLegacyLayoutIfNeeded();
  }

  if (TextAutosizer* autosizer = GetTextAutosizer())
    autosizer->UpdatePageInfo();

  frame_->DidAttachDocument();
  lifecycle_.AdvanceTo(DocumentLifecycle::kStyleClean);

  if (View())
    View()->DidAttachDocument();

  network_state_observer_ = MakeGarbageCollected<NetworkStateObserver>(*this);

  if (RuntimeEnabledFeatures::CompositeAfterPaintEnabled()) {
    CompositorAnimationTimeline* animation_timeline =
        Timeline().CompositorTimeline();
    if (animation_timeline) {
      GetPage()->GetChromeClient().AttachCompositorAnimationTimeline(
          animation_timeline, frame_.Get());
    }
  }
}
