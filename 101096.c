VirtualKeyboardController::VirtualKeyboardController()
    : has_external_keyboard_(false),
      has_internal_keyboard_(false),
      has_touchscreen_(false),
      ignore_external_keyboard_(false) {
  Shell::GetInstance()->AddShellObserver(this);
  ui::DeviceDataManager::GetInstance()->AddObserver(this);
  UpdateDevices();
}
