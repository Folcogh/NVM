#include "AnnouncementSound.hpp"
#include "Nvm.hpp"
#include <QDir>

AnnouncementSound* AnnouncementSound::Instance = nullptr;

AnnouncementSound::AnnouncementSound()
{
    QString SoundPath = QDir::currentPath() + '/' + ANNOUNCEMENT_PATH;   // Path of sound files
    QDir SoundDir     = QDir(SoundPath);                                 // Directory of sound files
    QStringList filters;                                                 // Files filter
    filters << "*.mp3";                                                  // Only mp3 files
    this->Sounds = SoundDir.entryList(filters, QDir::Files, QDir::Name); // Get *.mp3 from directory file
}

AnnouncementSound* AnnouncementSound::instance()
{
    if (Instance == nullptr) {
        Instance = new AnnouncementSound;
    }
    return Instance;
}

void AnnouncementSound::release()
{
    if (Instance != nullptr) {
        delete Instance;
        Instance = nullptr;
    }
}

bool AnnouncementSound::exists(QString name) const
{
    return this->soundNames().contains(name, Qt::CaseInsensitive);
}
