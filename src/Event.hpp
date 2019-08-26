#ifndef EVENT_HPP
#define EVENT_HPP

#include <QList>
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
    QString message() const { return Message; }
    QTime timecode() const { return Timecode; }
    QString sound() const { return Sound; }
    int countdown() const { return Countdown; }
    QList<QTime> announcements() const { return Announcements; }

    // Setters
    void setData(QString message, QTime timecode);
    void setData(QString message, QTime timecode, QString sound, int countdown, QList<QTime> announcements);

    // Shortcut used to display announcement status in a table
    bool hasAnnounce() const { return !Announcements.isEmpty() || (Countdown != 0); }

private:
    // Core
    QString Message; // Text displayed and read
    QTime Timecode;  // Time where event is triggered

    // Announcements
    QString Sound;              // Sound played for announcements
    int Countdown;              // Countdown announcement, in seconds. 0 means no countdown
    QList<QTime> Announcements; // Delays between announcements and event trigger
};

// Serialization
QDataStream& operator<<(QDataStream& stream, const Event& event); // Read from a file
QDataStream& operator>>(QDataStream& stream, Event& event);       // Write to a file

// We use Event* as QVariant to store a pointer to each event in the first column of the table
Q_DECLARE_METATYPE(Event*)

#endif // EVENT_HPP
