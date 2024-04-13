void RenderThreadImpl::SetFlingCurveParameters(
    const std::vector<float>& new_touchpad,
    const std::vector<float>& new_touchscreen) {
  webkit_platform_support_->SetFlingCurveParameters(new_touchpad,
                                                    new_touchscreen);

}
