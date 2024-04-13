FileReaderLoader::FileReaderLoader(
    ReadType read_type,
    FileReaderLoaderClient* client,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : read_type_(read_type),
      client_(client),
      handle_watcher_(FROM_HERE,
                      mojo::SimpleWatcher::ArmingPolicy::AUTOMATIC,
                      task_runner),
      binding_(this),
      task_runner_(std::move(task_runner)),
      weak_factory_(this) {}
