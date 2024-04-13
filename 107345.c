initStringBufferPool() {
	static widechar *stringBuffers[MAXPASSBUF] = { NULL };
	static int stringBuffersInUse[MAXPASSBUF] = { 0 };
	StringBufferPool *pool = malloc(sizeof(StringBufferPool));
	pool->size = MAXPASSBUF;
	pool->buffers = stringBuffers;
	pool->inUse = stringBuffersInUse;
	pool->alloc = &allocStringBuffer;
	pool->free = NULL;
	stringBufferPool = pool;
}
