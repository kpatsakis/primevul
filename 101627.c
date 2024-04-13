bool SQLiteDatabase::TurnOnIncrementalAutoVacuum() {
  SQLiteStatement statement(*this, "PRAGMA auto_vacuum");
  int auto_vacuum_mode = statement.GetColumnInt(0);
  int error = LastError();

  statement.Finalize();

  if (error != SQLITE_ROW)
    return false;

  switch (auto_vacuum_mode) {
    case kAutoVacuumIncremental:
      return true;
    case kAutoVacuumFull:
      return ExecuteCommand("PRAGMA auto_vacuum = 2");
    case kAutoVacuumNone:
    default:
      if (!ExecuteCommand("PRAGMA auto_vacuum = 2"))
        return false;
      RunVacuumCommand();
      error = LastError();
      return (error == SQLITE_OK);
  }
}
