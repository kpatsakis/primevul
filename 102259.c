 virtual status_t verify(Vector<uint8_t> const &sessionId,
 Vector<uint8_t> const &keyId,
 Vector<uint8_t> const &message,
 Vector<uint8_t> const &signature,
 bool &match) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, sessionId);
        writeVector(data, keyId);
        writeVector(data, message);
        writeVector(data, signature);

 status_t status = remote()->transact(VERIFY, data, &reply);
 if (status != OK) {
 return status;
 }
        match = (bool)reply.readInt32();
 return reply.readInt32();
 }
