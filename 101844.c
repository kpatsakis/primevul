GamepadProvider::ClosureAndThread::ClosureAndThread(
    const base::Closure& c,
    const scoped_refptr<base::SingleThreadTaskRunner>& m)
    : closure(c), task_runner(m) {}
