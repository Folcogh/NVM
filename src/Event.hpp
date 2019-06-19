#ifndef EVENT_HPP
#define EVENT_HPP

#include <QMetaType>
#include <QString>
#include <QTime>

//  Event
//
// This class contains event data: timecode, text to display, vocal announcements
//
class Event
{
public:
    // Getters
    QString message() const { return this->Message; }
    QTime timecode() const { return this->Timecode; }
    QTime firstAnnouncementDelay() const { return this->FirstAnnouncementDelay; }
    QTime secondAnnouncementDelay() const { return this->SecondAnnouncementDelay; }
    QTime thirdAnnouncementDelay() const { return this->ThirdAnnouncementDelay; }
    int finalCountdownDelay() const { return this->FinalCountdownDelay; }
    bool firstAnnouncement() const { return this->FirstAnnouncement; }
    bool secondAnnouncement() const { return this->SecondAnnouncement; }
    bool thirdAnnouncement() const { return this->ThirdAnnouncement; }
    bool finalCountdown() const { return this->FinalCountdown; }
    QString sound() const { return this->Sound; }

    // Setter
    void setData(QString message, QTime timecode, QTime firstd, QTime secondd, QTime thirdd, int finald, bool first, bool second,
                 bool third, bool final, QString sound);

    // Shortcut used to display announcement status in a table
    bool hasAnnounce() const { return firstAnnouncement() || secondAnnouncement() || thirdAnnouncement() || finalCountdown(); }

private:
    QString Message;          // Text displayed and read
    QTime Timecode;           // Time where event is triggered
    QTime FirstAnnouncementDelay; // Some delays before the announcement. May be not null even if the corresponding boolean is true
    QTime SecondAnnouncementDelay;
    QTime ThirdAnnouncementDelay;
    int FinalCountdownDelay; // Countdown announcmente, in seconds
    bool FirstAnnouncement;  // True if the announcement must be emitted
    bool SecondAnnouncement;
    bool ThirdAnnouncement;
    bool FinalCountdown;
    QString Sound;
};

// Serialization
QDataStream& operator<<(QDataStream& stream, const Event& event); // Read from a file
QDataStream& operator>>(QDataStream& stream, Event& event);       // Write to a file

// We use Event* as QVariant to store a pointer to each event in the first column of the table
Q_DECLARE_METATYPE(Event*)

#endif // EVENT_HPP
