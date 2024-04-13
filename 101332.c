void HTMLMediaElement::ActivateViewportIntersectionMonitoring(bool activate) {
  if (activate && !viewport_intersection_observer_) {
    viewport_intersection_observer_ = IntersectionObserver::Create(
        {}, {kMostlyFillViewportThreshold}, &(GetDocument()),
        WTF::BindRepeating(&HTMLMediaElement::OnViewportIntersectionChanged,
                           WrapWeakPersistent(this)),
        IntersectionObserver::kFractionOfRoot);
    viewport_intersection_observer_->observe(this);
  } else if (!activate && viewport_intersection_observer_) {
    viewport_intersection_observer_->disconnect();
    viewport_intersection_observer_ = nullptr;
    mostly_filling_viewport_ = false;
  }
}
