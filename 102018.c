void UiSceneCreator::CreateToasts() {
  auto* parent = AddTransientParent(kExclusiveScreenToastTransientParent,
                                    k2dBrowsingForeground, kToastTimeoutSeconds,
                                    false, scene_);
  parent->AddBinding(VR_BIND_FUNC(bool, Model, model_,
                                  fullscreen && !model->web_vr_mode, UiElement,
                                  parent, SetVisible));

  auto element = base::MakeUnique<ExclusiveScreenToast>(512);
  element->SetName(kExclusiveScreenToast);
  element->SetDrawPhase(kPhaseForeground);
  element->SetSize(kToastWidthDMM, kToastHeightDMM);
  element->SetTranslate(
      0,
      kFullscreenVerticalOffset + kFullscreenHeight / 2 +
          (kToastOffsetDMM + kToastHeightDMM) * kFullscreenToastDistance,
      -kFullscreenToastDistance);
  element->SetScale(kFullscreenToastDistance, kFullscreenToastDistance, 1);
  element->set_hit_testable(false);
  BindColor(model_, element.get(),
            &ColorScheme::exclusive_screen_toast_background,
            &TexturedElement::SetBackgroundColor);
  BindColor(model_, element.get(),
            &ColorScheme::exclusive_screen_toast_foreground,
            &TexturedElement::SetForegroundColor);
  scene_->AddUiElement(kExclusiveScreenToastTransientParent,
                       std::move(element));

  parent = AddTransientParent(kExclusiveScreenToastViewportAwareTransientParent,
                              kWebVrViewportAwareRoot, kToastTimeoutSeconds,
                              false, scene_);
  parent->AddBinding(
      VR_BIND_FUNC(bool, Model, model_,
                   web_vr_has_produced_frames() && model->web_vr_show_toast,
                   UiElement, parent, SetVisible));

  element = base::MakeUnique<ExclusiveScreenToast>(512);
  element->SetName(kExclusiveScreenToastViewportAware);
  element->SetDrawPhase(kPhaseOverlayForeground);
  element->SetSize(kToastWidthDMM, kToastHeightDMM);
  element->SetTranslate(0, kWebVrToastDistance * sin(kWebVrAngleRadians),
                        -kWebVrToastDistance * cos(kWebVrAngleRadians));
  element->SetRotate(1, 0, 0, kWebVrAngleRadians);
  element->SetScale(kWebVrToastDistance, kWebVrToastDistance, 1);
  element->set_hit_testable(false);
  BindColor(model_, element.get(),
            &ColorScheme::exclusive_screen_toast_background,
            &TexturedElement::SetBackgroundColor);
  BindColor(model_, element.get(),
            &ColorScheme::exclusive_screen_toast_foreground,
            &TexturedElement::SetForegroundColor);
  scene_->AddUiElement(kExclusiveScreenToastViewportAwareTransientParent,
                       std::move(element));
}
