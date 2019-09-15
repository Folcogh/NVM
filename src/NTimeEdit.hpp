#ifndef NTIMEEDIT_HPP
#define NTIMEEDIT_HPP

#include <QWidget>

namespace Ui {
class NTimeEdit;
}

class NTimeEdit : public QWidget
{
    Q_OBJECT

public:
    explicit NTimeEdit(QWidget *parent = nullptr);
    ~NTimeEdit();

private:
    Ui::NTimeEdit *ui;
};

#endif // NTIMEEDIT_HPP
