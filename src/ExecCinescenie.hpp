#ifndef EXECCINESCENIE_HPP
#define EXECCINESCENIE_HPP

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

    void deleteEvents();
    void buttonCloseClicked();

signals:
    void execCinescenieClosed();
};

// Column definition
#define COLUMN_TIMECODE 0
#define COLUMN_REMAINING 1
#define COLUMN_MESSAGE 2

#endif // EXECCINESCENIE_HPP
