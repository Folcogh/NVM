#ifndef EVENT_HPP
#define EVENT_HPP

#include <QMetaType>
#include <QString>
#include <QTime>

//  Event
//
// This class contains event data: timecode, text to display, vocal announces
//
class Event
{
public:
    // Getters
    QString message() const { return this->Message; }
    QTime timecode() const { return this->Timecode; }
    QTime firstAnnounceDelay() const { return this->FirstAnnounceDelay; }
    QTime secondAnnounceDelay() const { return this->SecondAnnounceDelay; }
    QTime thirdAnnounceDelay() const { return this->ThirdAnnounceDelay; }
    int finalCountdownDelay() const { return this->FinalCountdownDelay; }
    bool firstAnnounce() const { return this->FirstAnnounce; }
    bool secondAnnounce() const { return this->SecondAnnounce; }
    bool thirdAnnounce() const { return this->ThirdAnnounce; }
    bool finalCountdown() const { return this->FinalCountdown; }

    // Setter
    void setData(QString message, QTime timecode, QTime firstd, QTime secondd, QTime thirdd, int finald, bool first, bool second,
                 bool third, bool final);

    // Shortcut used to display announce status in a table
    bool hasAnnounce() const { return firstAnnounce() || secondAnnounce() || thirdAnnounce() || finalCountdown(); }

private:
    QString Message;          // Text displayed and read
    QTime Timecode;           // Time where event is triggered
    QTime FirstAnnounceDelay; // Some delays before the announce. May be not null even if the corresponding boolean is true
    QTime SecondAnnounceDelay;
    QTime ThirdAnnounceDelay;
    int FinalCountdownDelay; // Countdown announce, in seconds
    bool FirstAnnounce;      // True if the announce must be emitted
    bool SecondAnnounce;
    bool ThirdAnnounce;
    bool FinalCountdown;
};

// Serialization
QDataStream& operator<<(QDataStream& stream, const Event& event); // Read from a file
QDataStream& operator>>(QDataStream& stream, Event& event);       // Write to a file

// We use Event* as QVariant to store a pointer to each event in the first column of the table
Q_DECLARE_METATYPE(Event*)

#endif // EVENT_HPP
