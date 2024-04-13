int MacroExpander::expandEscapedMacro(const QString &str, int pos, QStringList &ret)
{
    ushort option = str[pos+1].unicode();

    switch (option) {
    case 'f': // Filepath
    case 'F': // case insensitive
        if (m_device.is<Solid::StorageAccess>()) {
            ret << m_device.as<Solid::StorageAccess>()->filePath();
        } else {
            qWarning() << "DeviceServiceAction::execute: " << m_device.udi()
                       << " is not a StorageAccess device";
        }
        break;
    case 'd': // Device node
    case 'D': // case insensitive
        if (m_device.is<Solid::Block>()) {
            ret << m_device.as<Solid::Block>()->device();
        } else {
            qWarning() << "DeviceServiceAction::execute: " << m_device.udi()
                       << " is not a Block device";
        }
        break;
    case 'i': // UDI
    case 'I': // case insensitive
        ret << m_device.udi();
        break;
    case '%':
        ret = QStringList(QLatin1String("%"));
        break;
    default:
        return -2; // subst with same and skip
    }
    return 2;
}
