#ifndef ANNOUNCEMENTSOUND_HPP
#define ANNOUNCEMENTSOUND_HPP

#include <QString>
#include <QStringList>

class AnnouncementSound
{
public:
    static AnnouncementSound* instance();
    static void release();
    QStringList soundNames() const { return this->Sounds; }
    bool exists(QString name) const;

private:
    static AnnouncementSound* Instance;
    QStringList Sounds;

    AnnouncementSound();
};

#endif // ANNOUNCEMENTSOUND_HPP
