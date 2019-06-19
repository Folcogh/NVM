#include "MainWindow.hpp"
#include "Nvm.hpp"
#include "ui_MainWindow.h"
#include <QCommandLinkButton>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , WidgetEdit(new EditCinescenie)
    , WidgetExec(new ExecCinescenie)
    , DefaultDirectory(QDir::homePath())
{
    ui->setupUi(this);

    // Add the possible widgets to the central widget
    centralWidget()->layout()->addWidget(WidgetEdit);
    centralWidget()->layout()->addWidget(WidgetExec);

    // Connect the Choice widget
    connect(ui->ButtonNew, &QCommandLinkButton::clicked, [this]() { newCinescenieTriggered(); });
    connect(ui->ButtonLoad, &QCommandLinkButton::clicked, [this]() { loadCinescenieTriggered(); });
    connect(ui->ButtonModify, &QCommandLinkButton::clicked, [this]() { modifyCinescenieTriggered(); });

    // Connect the Edit Cinescenie widget
    connect(this->WidgetEdit, &EditCinescenie::editCinescenieClosed, [this]() { displayChoice(); });

    // Connect the New Cinescenie widget
    connect(this->WidgetExec, &ExecCinescenie::execCinescenieClosed, [this]() { displayChoice(); });

    // Display only the choice widget
    displayChoice();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//  loadCinescenie
//
// Called by main.cpp when NVM is launched through a nvm file
//
void MainWindow::loadCinescenie(QString filename)
{
    // Try to open it, display it in case of success
    if (this->WidgetExec->execCinescenie(filename)) {
        this->WidgetExec->setVisible(true);
        ui->WidgetChoice->setVisible(false);
    }
}

void MainWindow::displayChoice()
{
    this->WidgetEdit->setVisible(false);
    this->WidgetExec->setVisible(false);
    ui->WidgetChoice->setVisible(true);
}

void MainWindow::newCinescenieTriggered()
{
    this->WidgetEdit->editNewCinescenie();
    this->WidgetEdit->setVisible(true);
    ui->WidgetChoice->setVisible(false);
}

void MainWindow::loadCinescenieTriggered()
{
    // Get the file to open
    QString filename = requestCinescenieFile(tr("Charger une cinéscénie existante"));
    if (filename.isEmpty()) {
        return;
    }

    loadCinescenie(filename);
}

void MainWindow::modifyCinescenieTriggered()
{
    // Get the file to open
    QString filename = requestCinescenieFile(tr("Modifier une cinéscénie existante"));
    if (filename.isEmpty()) {
        return;
    }

    // Try to open it, display it in case of success
    if (this->WidgetEdit->editCinescenie(filename)) {
        this->WidgetEdit->setVisible(true);
        ui->WidgetChoice->setVisible(false);
    }
}

//  requestCinescenieFile
//
// Prompt for a cinescenie file. Return a null-string if selection was cancelled
//
QString MainWindow::requestCinescenieFile(QString title)
{
    // Prompt for the file to load
    QString filename = QFileDialog::getOpenFileName(this, title, DefaultDirectory, tr("Fichier de cinéscénie (*." FILE_EXTENSION ")"));

    // Save the current directory if a file were chosen
    if (!filename.isEmpty()) {
        this->DefaultDirectory = QFileInfo(filename).canonicalPath();
    }

    return filename;
}
