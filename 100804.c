  NativeAppModalDialog* GetDialog() {
    AppModalDialog* dialog = ui_test_utils::WaitForAppModalDialog();
    EXPECT_TRUE(dialog->IsJavaScriptModalDialog());
    JavaScriptAppModalDialog* js_dialog =
        static_cast<JavaScriptAppModalDialog*>(dialog);
    NativeAppModalDialog* native_dialog = js_dialog->native_dialog();
    EXPECT_TRUE(native_dialog);
    return native_dialog;
  }
