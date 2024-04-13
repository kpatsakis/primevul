void FileSystemOperationRunner::DidFinish(const OperationID id,
                                          StatusCallback callback,
                                          base::File::Error rv) {
  if (is_beginning_operation_) {
    finished_operations_.insert(id);
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE, base::BindOnce(&FileSystemOperationRunner::DidFinish,
                                  weak_ptr_, id, std::move(callback), rv));
    return;
  }
  std::move(callback).Run(rv);
  FinishOperation(id);
}
