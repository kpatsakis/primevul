void RenderWidgetHostImpl::OnShowDisambiguationPopup(
    const gfx::Rect& rect_pixels,
    const gfx::Size& size,
    const cc::SharedBitmapId& id) {
  DCHECK(!rect_pixels.IsEmpty());
  DCHECK(!size.IsEmpty());

  scoped_ptr<cc::SharedBitmap> bitmap =
      HostSharedBitmapManager::current()->GetSharedBitmapFromId(size, id);
  if (!bitmap) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RWH_SHARED_BITMAP);
    return;
  }

  DCHECK(bitmap->pixels());

  SkImageInfo info = SkImageInfo::MakeN32Premul(size.width(), size.height());
  SkBitmap zoomed_bitmap;
  zoomed_bitmap.installPixels(info, bitmap->pixels(), info.minRowBytes());

  if (view_)
    view_->ShowDisambiguationPopup(rect_pixels, zoomed_bitmap);

  zoomed_bitmap.setPixels(0);
  Send(new ViewMsg_ReleaseDisambiguationPopupBitmap(GetRoutingID(), id));
}
