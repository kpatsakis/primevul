 virtual status_t setCipherAlgorithm(Vector<uint8_t> const &sessionId,
 String8 const &algorithm) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, sessionId);
        data.writeString8(algorithm);
 status_t status = remote()->transact(SET_CIPHER_ALGORITHM, data, &reply);
 if (status != OK) {
 return status;
 }
 return reply.readInt32();
 }
