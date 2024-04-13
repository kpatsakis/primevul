 virtual status_t setObserver(const sp<IHDCPObserver> &observer) {
 Parcel data, reply;
        data.writeInterfaceToken(IHDCP::getInterfaceDescriptor());
        data.writeStrongBinder(observer->asBinder());
        remote()->transact(HDCP_SET_OBSERVER, data, &reply);
 return reply.readInt32();
 }
