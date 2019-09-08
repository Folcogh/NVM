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
    bool loadCinescenie(QString filename);

private:
    Ui::ExecCinescenie* ui;
    bool Started = false;
    QTime StartTime;
    QTime InitialTimecode;
    int TimerID;
    int CurrentEvent;

    inline Event* nvmEvent(int row) const;
    void deleteEvents();
    void buttonStartClicked();
    void buttonCloseClicked();
    void checkboxDisplayTimecodeToggled();

    void timerEvent(QTimerEvent*);

signals:
    void execCinescenieClosed();
};

// Column definition
#define EXEC_COLUMN_TIMECODE 0
#define EXEC_COLUMN_REMAINING 1
#define EXEC_COLUMN_MESSAGE 2

// Interval of refresh timer (in ms)
#define INTERVAL_UI_TIMER 200

#endif // EXECCINESCENIE_HPP
