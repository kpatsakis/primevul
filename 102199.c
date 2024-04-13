ATSParser::Program::Program(
 ATSParser *parser, unsigned programNumber, unsigned programMapPID,
 int64_t lastRecoveredPTS)
 : mParser(parser),
      mProgramNumber(programNumber),
      mProgramMapPID(programMapPID),
      mFirstPTSValid(false),
      mFirstPTS(0),
      mLastRecoveredPTS(lastRecoveredPTS) {
    ALOGV("new program number %u", programNumber);
}
