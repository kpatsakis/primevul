void CSoundFile::PortamentoMPT(ModChannel* pChn, int param)
{

	pChn->m_PortamentoFineSteps += param;
	pChn->m_CalculateFreq = true;
}
