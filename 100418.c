void JSArray::fillArgList(ExecState* exec, MarkedArgumentBuffer& args)
{
    ArrayStorage* storage = m_storage;

    WriteBarrier<Unknown>* vector = storage->m_vector;
    unsigned vectorEnd = min(storage->m_length, m_vectorLength);
    unsigned i = 0;
    for (; i < vectorEnd; ++i) {
        WriteBarrier<Unknown>& v = vector[i];
        if (!v)
            break;
        args.append(v.get());
    }

    for (; i < storage->m_length; ++i)
        args.append(get(exec, i));
}
