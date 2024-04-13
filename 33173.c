QString DeviceServiceAction::id() const
{
    if (m_service.name().isEmpty() && m_service.exec().isEmpty()) {
        return QString();
    } else {
        return "#Service:"+m_service.name()+m_service.exec();
    }
}
