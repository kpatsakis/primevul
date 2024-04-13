void CSoundFile::PortamentoFineMPT(ModChannel* pChn, int param)
{

	if(m_PlayState.m_nTickCount == 0)
		pChn->nOldFinePortaUpDown = 0;

	const int tickParam = static_cast<int>((m_PlayState.m_nTickCount + 1.0) * param / m_PlayState.m_nMusicSpeed);
	pChn->m_PortamentoFineSteps += (param >= 0) ? tickParam - pChn->nOldFinePortaUpDown : tickParam + pChn->nOldFinePortaUpDown;
	if(m_PlayState.m_nTickCount + 1 == m_PlayState.m_nMusicSpeed)
		pChn->nOldFinePortaUpDown = static_cast<int8>(mpt::abs(param));
	else
		pChn->nOldFinePortaUpDown = static_cast<int8>(mpt::abs(tickParam));

	pChn->m_CalculateFreq = true;
}
