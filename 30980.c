PickKeyboard(ClientPtr client)
{
    DeviceIntPtr ptr = PickPointer(client);
    DeviceIntPtr kbd = GetMaster(ptr, MASTER_KEYBOARD);

    if (!kbd) {
        ErrorF("[dix] ClientPointer not paired with a keyboard. This "
               "is a bug.\n");
    }

    return kbd;
}
