releaseStringBuffer(int idx) {
	if (idx >= 0 && idx < stringBufferPool->size) {
		int inUse = stringBufferPool->inUse[idx];
		if (inUse && stringBufferPool->free)
			stringBufferPool->free(stringBufferPool->buffers[idx]);
		stringBufferPool->inUse[idx] = 0;
		return inUse;
	}
	return 0;
}
