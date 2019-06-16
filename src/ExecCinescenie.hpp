#ifndef EXECCINESCENIE_HPP
#define EXECCINESCENIE_HPP

#include "Event.hpp"
#include <QTime>
#include <QWidget>

namespace Ui {
class ExecCinescenie;
}

class ExecCinescenie : public QWidget
{
    Q_OBJECT

public:
    explicit ExecCinescenie(QWidget *parent = nullptr);
    ~ExecCinescenie();
    bool execCinescenie(QString filename);

private:
    Ui::ExecCinescenie* ui;
    bool Started = false;
    QTime StartTime;
    int TimerID;

    Event* nvmEvent(int row) const;
    void deleteEvents();
    void buttonStartClicked();
    void buttonCloseClicked();

    void timerEvent(QTimerEvent*);

signals:
    void execCinescenieClosed();
};

// Column definition
#define COLUMN_TIMECODE 0
#define COLUMN_REMAINING 1
#define COLUMN_MESSAGE 2

// Interval of refresh timer (in ms)
#define INTERVAL_UI_TIMER 200

#endif // EXECCINESCENIE_HPP
