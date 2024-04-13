DeviceServiceAction::DeviceServiceAction()
    : DeviceAction()
{
    DeviceAction::setIconName(QStringLiteral("dialog-cancel"));
    DeviceAction::setLabel(i18nc("A default name for an action without proper label", "Unknown"));
}
