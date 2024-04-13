void DeviceServiceAction::setService(const KServiceAction& service)
{
    DeviceAction::setIconName(service.icon());
    DeviceAction::setLabel(service.text());

    m_service = service;
}
