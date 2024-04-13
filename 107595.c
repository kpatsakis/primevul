void CSoundFile::InvertLoop(ModChannel *pChn)
{
	if(GetType() != MOD_TYPE_MOD || pChn->nEFxSpeed == 0) return;

	ModSample *pModSample = const_cast<ModSample *>(pChn->pModSample);
	if(pModSample == nullptr || !pModSample->HasSampleData() || !pModSample->uFlags[CHN_LOOP] || pModSample->uFlags[CHN_16BIT]) return;

	pChn->nEFxDelay += ModEFxTable[pChn->nEFxSpeed & 0x0F];
	if((pChn->nEFxDelay & 0x80) == 0) return; // only applied if the "delay" reaches 128
	pChn->nEFxDelay = 0;

	if (++pChn->nEFxOffset >= pModSample->nLoopEnd - pModSample->nLoopStart)
		pChn->nEFxOffset = 0;

	uint8 &sample = mpt::byte_cast<uint8 *>(pModSample->sampleb())[pModSample->nLoopStart + pChn->nEFxOffset];
	sample = ~sample;
	ctrlSmp::PrecomputeLoops(*pModSample, *this, false);
}
