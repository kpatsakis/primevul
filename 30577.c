dbus_bool_t _dbus_read_local_machine_uuid   (DBusGUID         *machine_id,
                                             dbus_bool_t       create_if_not_found,
                                             DBusError        *error)
{
#ifdef DBUS_WINCE
	return TRUE;
#else
    HW_PROFILE_INFOA info;
    char *lpc = &info.szHwProfileGuid[0];
    dbus_uint32_t u;

    if(!GetCurrentHwProfileA(&info))
      {
        dbus_set_error (error, DBUS_ERROR_NO_MEMORY, NULL); // FIXME
        return FALSE;  
      }

    lpc++;
    u = ((fromAscii(lpc[0]) <<  0) |
         (fromAscii(lpc[1]) <<  4) |
         (fromAscii(lpc[2]) <<  8) |
         (fromAscii(lpc[3]) << 12) |
         (fromAscii(lpc[4]) << 16) |
         (fromAscii(lpc[5]) << 20) |
         (fromAscii(lpc[6]) << 24) |
         (fromAscii(lpc[7]) << 28));
    machine_id->as_uint32s[0] = u;

    lpc += 9;
    u = ((fromAscii(lpc[0]) <<  0) |
         (fromAscii(lpc[1]) <<  4) |
         (fromAscii(lpc[2]) <<  8) |
         (fromAscii(lpc[3]) << 12) |
         (fromAscii(lpc[5]) << 16) |
         (fromAscii(lpc[6]) << 20) |
         (fromAscii(lpc[7]) << 24) |
         (fromAscii(lpc[8]) << 28));
    machine_id->as_uint32s[1] = u;
    
    lpc += 10;
    u = ((fromAscii(lpc[0]) <<  0) |
         (fromAscii(lpc[1]) <<  4) |
         (fromAscii(lpc[2]) <<  8) |
         (fromAscii(lpc[3]) << 12) |
         (fromAscii(lpc[5]) << 16) |
         (fromAscii(lpc[6]) << 20) |
         (fromAscii(lpc[7]) << 24) |
         (fromAscii(lpc[8]) << 28));
    machine_id->as_uint32s[2] = u;
    
    lpc += 9;
    u = ((fromAscii(lpc[0]) <<  0) |
         (fromAscii(lpc[1]) <<  4) |
         (fromAscii(lpc[2]) <<  8) |
         (fromAscii(lpc[3]) << 12) |
         (fromAscii(lpc[4]) << 16) |
         (fromAscii(lpc[5]) << 20) |
         (fromAscii(lpc[6]) << 24) |
         (fromAscii(lpc[7]) << 28));
    machine_id->as_uint32s[3] = u;
#endif
    return TRUE;
}
