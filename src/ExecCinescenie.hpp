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

signals:
    void execCinescenieClosed();
};

#endif // EXECCINESCENIE_HPP
