#include "Event.hpp"
#include <QDataStream>

//  setData (without announcement)
//
// Fill an event with data. Used on event creation, modification, or when its read from a file0
//
void Event::setData(QString message, QTime timecode)
{
    this->Message       = message;
    this->Timecode      = timecode;
    this->Sound         = QString();
    this->Countdown     = 0;
    this->Announcements = QList<QTime>();
}

//  setData (with announcement)
//
// Fill an event with data. Used on event creation, modification, or when its read from a file0
//
void Event::setData(QString message, QTime timecode, QString sound, int countdown, QList<QTime> announcements)
{
    this->Message       = message;
    this->Timecode      = timecode;
    this->Sound         = sound;
    this->Countdown     = countdown;
    this->Announcements = announcements;
}

//  <<
//
// Write an event into a stream. Stream status is not checked
//
QDataStream& operator<<(QDataStream& stream, const Event& event)
{
    stream << event.message() << event.timecode() << event.sound() << event.countdown() << event.announcements();
    return stream;
}

//  >>
//
// Read an event from a stream. Stream status is not checked
//
QDataStream& operator>>(QDataStream& stream, Event& event)
{
    QString message;
    QTime timecode;
    QString sound;
    int countdown;
    QList<QTime> announcements;

    stream >> message >> timecode >> sound >> countdown >> announcements;
    event.setData(message, timecode, sound, countdown, announcements);
    return stream;
}
