#include "Event.hpp"
#include <QDataStream>

//  setData
//
// Fill an event with data. Used on event creation, modification, or when its read from a file
//
void Event::setData(QString message, QTime timecode, QTime firstd, QTime secondd, QTime thirdd, int finald, bool first, bool second,
                    bool third, bool final, QString sound)
{
    this->Message                 = message;
    this->Timecode                = timecode;
    this->FirstAnnouncementDelay  = firstd;
    this->SecondAnnouncementDelay = secondd;
    this->ThirdAnnouncementDelay  = thirdd;
    this->FinalCountdownDelay     = finald;
    this->FirstAnnouncement       = first;
    this->SecondAnnouncement      = second;
    this->ThirdAnnouncement       = third;
    this->FinalCountdown          = final;
    this->Sound                   = sound;
}

//  <<
//
// Write an event into a stream. Stream status not checked
//
QDataStream& operator<<(QDataStream& stream, const Event& event)
{
    stream << event.message() << event.timecode() << event.firstAnnouncementDelay() << event.secondAnnouncementDelay()
           << event.thirdAnnouncementDelay() << event.finalCountdownDelay() << event.firstAnnouncement() << event.secondAnnouncement()
           << event.thirdAnnouncement() << event.finalCountdown() << event.sound();
    return stream;
}

//  >>
//
// Read an event from a stream. Stream status not checked
//
QDataStream& operator>>(QDataStream& stream, Event& event)
{
    QString message;
    QTime timecode;
    QTime firstd;
    QTime secondd;
    QTime thirdd;
    int finald;
    bool first;
    bool second;
    bool third;
    bool final;
    QString sound;

    stream >> message >> timecode >> firstd >> secondd >> thirdd >> finald >> first >> second >> third >> final >> sound;
    event.setData(message, timecode, firstd, secondd, thirdd, finald, first, second, third, final, sound);
    return stream;
}
