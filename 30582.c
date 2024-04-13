_dbus_strerror (int error_number)
{
#ifdef DBUS_WINCE
  return "unknown";
#else
  const char *msg;

  switch (error_number)
    {
    case WSAEINTR:
      return "Interrupted function call";
    case WSAEACCES:
      return "Permission denied";
    case WSAEFAULT:
      return "Bad address";
    case WSAEINVAL:
      return "Invalid argument";
    case WSAEMFILE:
      return "Too many open files";
    case WSAEWOULDBLOCK:
      return "Resource temporarily unavailable";
    case WSAEINPROGRESS:
      return "Operation now in progress";
    case WSAEALREADY:
      return "Operation already in progress";
    case WSAENOTSOCK:
      return "Socket operation on nonsocket";
    case WSAEDESTADDRREQ:
      return "Destination address required";
    case WSAEMSGSIZE:
      return "Message too long";
    case WSAEPROTOTYPE:
      return "Protocol wrong type for socket";
    case WSAENOPROTOOPT:
      return "Bad protocol option";
    case WSAEPROTONOSUPPORT:
      return "Protocol not supported";
    case WSAESOCKTNOSUPPORT:
      return "Socket type not supported";
    case WSAEOPNOTSUPP:
      return "Operation not supported";
    case WSAEPFNOSUPPORT:
      return "Protocol family not supported";
    case WSAEAFNOSUPPORT:
      return "Address family not supported by protocol family";
    case WSAEADDRINUSE:
      return "Address already in use";
    case WSAEADDRNOTAVAIL:
      return "Cannot assign requested address";
    case WSAENETDOWN:
      return "Network is down";
    case WSAENETUNREACH:
      return "Network is unreachable";
    case WSAENETRESET:
      return "Network dropped connection on reset";
    case WSAECONNABORTED:
      return "Software caused connection abort";
    case WSAECONNRESET:
      return "Connection reset by peer";
    case WSAENOBUFS:
      return "No buffer space available";
    case WSAEISCONN:
      return "Socket is already connected";
    case WSAENOTCONN:
      return "Socket is not connected";
    case WSAESHUTDOWN:
      return "Cannot send after socket shutdown";
    case WSAETIMEDOUT:
      return "Connection timed out";
    case WSAECONNREFUSED:
      return "Connection refused";
    case WSAEHOSTDOWN:
      return "Host is down";
    case WSAEHOSTUNREACH:
      return "No route to host";
    case WSAEPROCLIM:
      return "Too many processes";
    case WSAEDISCON:
      return "Graceful shutdown in progress";
    case WSATYPE_NOT_FOUND:
      return "Class type not found";
    case WSAHOST_NOT_FOUND:
      return "Host not found";
    case WSATRY_AGAIN:
      return "Nonauthoritative host not found";
    case WSANO_RECOVERY:
      return "This is a nonrecoverable error";
    case WSANO_DATA:
      return "Valid name, no data record of requested type";
    case WSA_INVALID_HANDLE:
      return "Specified event object handle is invalid";
    case WSA_INVALID_PARAMETER:
      return "One or more parameters are invalid";
    case WSA_IO_INCOMPLETE:
      return "Overlapped I/O event object not in signaled state";
    case WSA_IO_PENDING:
      return "Overlapped operations will complete later";
    case WSA_NOT_ENOUGH_MEMORY:
      return "Insufficient memory available";
    case WSA_OPERATION_ABORTED:
      return "Overlapped operation aborted";
#ifdef WSAINVALIDPROCTABLE

    case WSAINVALIDPROCTABLE:
      return "Invalid procedure table from service provider";
#endif
#ifdef WSAINVALIDPROVIDER

    case WSAINVALIDPROVIDER:
      return "Invalid service provider version number";
#endif
#ifdef WSAPROVIDERFAILEDINIT

    case WSAPROVIDERFAILEDINIT:
      return "Unable to initialize a service provider";
#endif

    case WSASYSCALLFAILURE:
      return "System call failure";
    }
  msg = strerror (error_number);
  if (msg == NULL)
    msg = "unknown";

  return msg;
#endif //DBUS_WINCE
}
