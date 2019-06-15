#include "Event.hpp"
#include <QDataStream>

//  setData
//
// Fill an event with data. Used on event creation, modification, or when its read from a file
//
void Event::setData(QString message, QTime timecode, QTime firstd, QTime secondd, QTime thirdd, int finald, bool first, bool second,
                    bool third, bool final)
{
    this->Message             = message;
    this->Timecode            = timecode;
    this->FirstAnnounceDelay  = firstd;
    this->SecondAnnounceDelay = secondd;
    this->ThirdAnnounceDelay  = thirdd;
    this->FinalCountdownDelay = finald;
    this->FirstAnnounce       = first;
    this->SecondAnnounce      = second;
    this->ThirdAnnounce       = third;
    this->FinalCountdown      = final;
}

//  <<
//
// Write an event into a stream. Stream status not checked
//
QDataStream& operator<<(QDataStream& stream, const Event& event)
{
    stream << event.message() << event.timecode() << event.firstAnnounceDelay() << event.secondAnnounceDelay() << event.thirdAnnounceDelay()
           << event.finalCountdownDelay() << event.firstAnnounce() << event.secondAnnounce() << event.thirdAnnounce()
           << event.finalCountdown();
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

    stream >> message >> timecode >> firstd >> secondd >> thirdd >> finald >> first >> second >> third >> final;
    event.setData(message, timecode, firstd, secondd, thirdd, finald, first, second, third, final);
    return stream;
}
