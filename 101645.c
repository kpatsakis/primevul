  ImeAdapterAndroid* ime_adapter() {
    return static_cast<RenderWidgetHostViewAndroid*>(
               web_contents()->GetRenderWidgetHostView())
        ->ime_adapter_for_testing();
  }
