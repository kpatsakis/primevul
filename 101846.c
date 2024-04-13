void GamepadProvider::OnGamepadConnectionChange(bool connected,
                                                int index,
                                                const Gamepad& pad) {
  if (connection_change_client_)
    connection_change_client_->OnGamepadConnectionChange(connected, index, pad);
}
