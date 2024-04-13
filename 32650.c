void Part::slotActivated(const QModelIndex &index)
{
    Q_UNUSED(index)

    if (QGuiApplication::keyboardModifiers() != Qt::ShiftModifier &&
        QGuiApplication::keyboardModifiers() != Qt::ControlModifier) {
        ArkSettings::defaultOpenAction() == ArkSettings::EnumDefaultOpenAction::Preview ? slotOpenEntry(Preview) : slotOpenEntry(OpenFile);
    }
}
