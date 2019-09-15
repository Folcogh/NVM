#include "NTimeEdit.hpp"
#include "ui_NTimeEdit.h"

NTimeEdit::NTimeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NTimeEdit)
{
    ui->setupUi(this);
}

NTimeEdit::~NTimeEdit()
{
    delete ui;
}
