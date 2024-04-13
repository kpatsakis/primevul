static QString findImageForSpecImagePath(const QString &_path)
{
    QString path = _path;
    if (path.startsWith(QLatin1String("file:"))) {
        QUrl url(path);
        path = url.toLocalFile();
    }
    return KIconLoader::global()->iconPath(path, -KIconLoader::SizeHuge,
                                           true /* canReturnNull */);
}
