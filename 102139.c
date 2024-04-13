void OMXCodec::fillOutputBuffer(IOMX::buffer_id buffer) {
 Vector<BufferInfo> *buffers = &mPortBuffers[kPortIndexOutput];
 for (size_t i = 0; i < buffers->size(); ++i) {
 if ((*buffers)[i].mBuffer == buffer) {
            fillOutputBuffer(&buffers->editItemAt(i));
 return;
 }
 }

    CHECK(!"should not be here.");
}
