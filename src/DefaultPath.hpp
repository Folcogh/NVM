#ifndef DEFAULTPATH_HPP
#define DEFAULTPATH_HPP

#include <QString>

class DefaultPath
{
public:
    static DefaultPath* instance();
    static void release();
    QString defaultPath() const;
    void setDefaultPath(QString filename);

private:
    static DefaultPath* defaultpath;
    DefaultPath();
    QString path;
};

#endif // DEFAULTPATH_HPP
