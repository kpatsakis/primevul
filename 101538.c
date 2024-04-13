void PDFiumEngine::Paint(const pp::Rect& rect,
                         pp::ImageData* image_data,
                         std::vector<pp::Rect>* ready,
                         std::vector<pp::Rect>* pending) {
  DCHECK(image_data);
  DCHECK(ready);
  DCHECK(pending);

  pp::Rect leftover = rect;
  for (size_t i = 0; i < visible_pages_.size(); ++i) {
    int index = visible_pages_[i];
    pp::Rect page_rect = pages_[index]->rect();
    pp::Rect page_rect_in_screen = GetPageScreenRect(index);
    pp::Rect dirty_in_screen = page_rect_in_screen.Intersect(leftover);
    if (dirty_in_screen.IsEmpty())
      continue;

    if (i == 0) {
      pp::Rect blank_space_in_screen = dirty_in_screen;
      blank_space_in_screen.set_y(0);
      blank_space_in_screen.set_height(dirty_in_screen.y());
      leftover = leftover.Subtract(blank_space_in_screen);
    }
    leftover = leftover.Subtract(dirty_in_screen);

    if (pages_[index]->available()) {
      int progressive = GetProgressiveIndex(index);
      if (progressive != -1) {
        DCHECK_GE(progressive, 0);
        DCHECK_LT(static_cast<size_t>(progressive), progressive_paints_.size());
        if (progressive_paints_[progressive].rect != dirty_in_screen) {
          pending->push_back(dirty_in_screen);
          continue;
        }
      }

      if (progressive == -1) {
        progressive = StartPaint(index, dirty_in_screen);
        progressive_paint_timeout_ = kMaxInitialProgressivePaintTimeMs;
      } else {
        progressive_paint_timeout_ = kMaxProgressivePaintTimeMs;
      }

      progressive_paints_[progressive].painted_ = true;
      if (ContinuePaint(progressive, image_data)) {
        FinishPaint(progressive, image_data);
        ready->push_back(dirty_in_screen);
      } else {
        pending->push_back(dirty_in_screen);
      }
    } else {
      PaintUnavailablePage(index, dirty_in_screen, image_data);
      ready->push_back(dirty_in_screen);
    }
  }
}
