#include "ExecCinescenie.hpp"
#include "ui_ExecCinescenie.h"

ExecCinescenie::ExecCinescenie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExecCinescenie)
{
    ui->setupUi(this);
}

ExecCinescenie::~ExecCinescenie()
{
    delete ui;
}

bool ExecCinescenie::execCinescenie(QString filename)
{
    (void)filename;
    return true;
}
