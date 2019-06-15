#ifndef EXECEVENT_HPP
#define EXECEVENT_HPP

#include <QString>
#include <QTime>

class ExecEvent
{
public:
    ExecEvent(QTime timecode, QString message)
        : Timecode(timecode)
        , Message(message)
    {
    }

    QTime timecode() const { return this->Timecode; }
    QString message() const { return this->Message; }

private:
    QTime Timecode;
    QString Message;
};

#endif // EXECEVENT_HPP
