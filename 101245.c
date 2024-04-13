void OutOfProcessInstance::OnPaint(
    const std::vector<pp::Rect>& paint_rects,
    std::vector<PaintManager::ReadyRect>* ready,
    std::vector<pp::Rect>* pending) {
  if (image_data_.is_null()) {
    DCHECK(plugin_size_.IsEmpty());
    return;
  }
  if (first_paint_) {
    first_paint_ = false;
    pp::Rect rect = pp::Rect(pp::Point(), image_data_.size());
    FillRect(rect, background_color_);
    ready->push_back(PaintManager::ReadyRect(rect, image_data_, true));
  }

  if (!received_viewport_message_)
    return;

  engine_->PrePaint();

  for (size_t i = 0; i < paint_rects.size(); i++) {
    pp::Rect rect =
        paint_rects[i].Intersect(pp::Rect(pp::Point(), plugin_size_));
    if (rect.IsEmpty())
      continue;

    pp::Rect pdf_rect = available_area_.Intersect(rect);
    if (!pdf_rect.IsEmpty()) {
      pdf_rect.Offset(available_area_.x() * -1, 0);

      std::vector<pp::Rect> pdf_ready;
      std::vector<pp::Rect> pdf_pending;
      engine_->Paint(pdf_rect, &image_data_, &pdf_ready, &pdf_pending);
      for (size_t j = 0; j < pdf_ready.size(); ++j) {
        pdf_ready[j].Offset(available_area_.point());
        ready->push_back(
            PaintManager::ReadyRect(pdf_ready[j], image_data_, false));
      }
      for (size_t j = 0; j < pdf_pending.size(); ++j) {
        pdf_pending[j].Offset(available_area_.point());
        pending->push_back(pdf_pending[j]);
      }
    }

    int32_t first_page_ypos = engine_->GetNumberOfPages() == 0 ?
        0 : engine_->GetPageScreenRect(0).y();
    if (rect.y() < first_page_ypos) {
      pp::Rect region = rect.Intersect(pp::Rect(
          pp::Point(), pp::Size(plugin_size_.width(), first_page_ypos)));
      ready->push_back(PaintManager::ReadyRect(region, image_data_, false));
      FillRect(region, background_color_);
    }

    for (size_t j = 0; j < background_parts_.size(); ++j) {
      pp::Rect intersection = background_parts_[j].location.Intersect(rect);
      if (!intersection.IsEmpty()) {
        FillRect(intersection, background_parts_[j].color);
        ready->push_back(
            PaintManager::ReadyRect(intersection, image_data_, false));
      }
    }
  }

  engine_->PostPaint();
}
