 virtual status_t useBuffer(
            node_id node, OMX_U32 port_index, const sp<IMemory> &params,
            buffer_id *buffer) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        data.writeStrongBinder(params->asBinder());
        remote()->transact(USE_BUFFER, data, &reply);

 status_t err = reply.readInt32();
 if (err != OK) {
 *buffer = 0;

 return err;
 }

 *buffer = (buffer_id)reply.readInt32();

 return err;
 }
