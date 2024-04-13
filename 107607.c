void CSoundFile::PortamentoExtraFineMPT(ModChannel* pChn, int param)
{

	if(pChn->isFirstTick)
	{
		pChn->m_PortamentoFineSteps += param;
		pChn->m_CalculateFreq = true;
	}
}
