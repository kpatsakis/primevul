MockOverscrollController* MockOverscrollController::Create(
    RenderWidgetHostView* rwhv) {
  std::unique_ptr<MockOverscrollControllerImpl> mock =
      std::make_unique<MockOverscrollControllerImpl>();
  MockOverscrollController* raw_mock = mock.get();

  RenderWidgetHostViewAura* rwhva =
      static_cast<RenderWidgetHostViewAura*>(rwhv);
  rwhva->SetOverscrollControllerForTesting(std::move(mock));

  return raw_mock;
}
