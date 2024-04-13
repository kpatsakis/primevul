 virtual void disconnect() {
 Parcel data, reply;
        data.writeInterfaceToken(
 IMediaHTTPConnection::getInterfaceDescriptor());

        remote()->transact(DISCONNECT, data, &reply);
 }
