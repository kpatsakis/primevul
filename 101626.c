int64_t SQLiteDatabase::LastInsertRowID() {
  if (!db_)
    return 0;
  return sqlite3_last_insert_rowid(db_);
}
