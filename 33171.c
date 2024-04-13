void DeviceServiceAction::execute(Solid::Device &device)
{
    new DelayedExecutor(m_service, device);
}
