getStringBuffer(int length) {
	int i;
	for (i = 0; i < stringBufferPool->size; i++) {
		if (!stringBufferPool->inUse[i]) {
			stringBufferPool->buffers[i] = stringBufferPool->alloc(i, length);
			stringBufferPool->inUse[i] = 1;
			return i;
		}
	}
	_lou_outOfMemory();
	return -1;
}
