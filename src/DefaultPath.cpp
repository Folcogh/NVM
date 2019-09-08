#include "DefaultPath.hpp"
#include <QDir>
#include <QFileInfo>

DefaultPath* DefaultPath::defaultpath = nullptr;

DefaultPath* DefaultPath::instance()
{
    if (defaultpath == nullptr) {
        defaultpath = new DefaultPath;
    }
    return defaultpath;
}

void DefaultPath::release()
{
    if (defaultpath != nullptr) {
        delete defaultpath;
        defaultpath = nullptr;
    }
}

DefaultPath::DefaultPath()
{
    this->path = QDir::homePath();
}

QString DefaultPath::defaultPath() const
{
    return this->path;
}

void DefaultPath::setDefaultPath(QString filename)
{
    this->path = QFileInfo(filename).canonicalPath();
}
