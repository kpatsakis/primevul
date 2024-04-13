 virtual status_t createPlugin(const uint8_t uuid[16]) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());
        data.write(uuid, 16);

 status_t status = remote()->transact(CREATE_PLUGIN, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }
