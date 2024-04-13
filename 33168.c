DelayedExecutor::DelayedExecutor(const KServiceAction &service, Solid::Device &device)
    : m_service(service)
{
    if (device.is<Solid::StorageAccess>()
            && !device.as<Solid::StorageAccess>()->isAccessible()) {
        Solid::StorageAccess *access = device.as<Solid::StorageAccess>();

        connect(access, &Solid::StorageAccess::setupDone,
                this, &DelayedExecutor::_k_storageSetupDone);

        access->setup();
    } else {
        delayedExecute(device.udi());
    }
}
