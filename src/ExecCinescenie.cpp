#include "ExecCinescenie.hpp"
#include "Nvm.hpp"
#include "ui_ExecCinescenie.h"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QVariant>

ExecCinescenie::ExecCinescenie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExecCinescenie)
{
    ui->setupUi(this);

    // Button connections
    connect(ui->ButtonStart, &QPushButton::clicked, [this]() { buttonStartClicked(); });
    connect(ui->ButtonClose, &QPushButton::clicked, [this]() { buttonCloseClicked(); });
}

ExecCinescenie::~ExecCinescenie()
{
    deleteEvents();
    delete ui;
}

void ExecCinescenie::deleteEvents()
{
    for (int row = 0; row < ui->TableEvents->rowCount(); row++) {
        delete nvmEvent(row);
    }
}

Event* ExecCinescenie::nvmEvent(int row) const
{
    return ui->TableEvents->item(row, COLUMN_TIMECODE)->data(EVENT_ROLE).value<Event*>();
}

bool ExecCinescenie::execCinescenie(QString filename)
{
    // Delete the previous cinescenie if one existed before
    deleteEvents();
    ui->TableEvents->clearContents();
    ui->TableEvents->setRowCount(0);

    // Open file and check for success
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier %1").arg(filename));
        return false;
    }

    // Create the associated stream, and read it to fill the table
    QDataStream stream(&file);
    while (!stream.atEnd()) {
        // Create the event and fill it
        Event* event = new Event;
        stream >> *event;

        // Add the event to the table
        QTableWidgetItem* timecode  = new QTableWidgetItem(event->timecode().toString());
        QTableWidgetItem* remaining = new QTableWidgetItem;
        QTableWidgetItem* message   = new QTableWidgetItem(event->message());

        // Widget settings
        timecode->setData(EVENT_ROLE, QVariant::fromValue(event));
        timecode->setTextAlignment(Qt::AlignCenter);
        remaining->setTextAlignment(Qt::AlignCenter);

        // Add a row and put items
        int row = ui->TableEvents->rowCount();
        ui->TableEvents->setRowCount(row + 1);
        ui->TableEvents->setItem(row, COLUMN_TIMECODE, timecode);
        ui->TableEvents->setItem(row, COLUMN_REMAINING, remaining);
        ui->TableEvents->setItem(row, COLUMN_MESSAGE, message);

        // Check if the stream is alive
        if (stream.status() != QDataStream::Ok) {
            QMessageBox::critical(this, tr("Erreur"), tr("Impossible de lire le fichier %1").arg(filename));
            return false;
        }
    }

    // Update UI
    ui->FrameDefineTimecode->setVisible(true);                      // Show the setting frame, allowing to set the timecode
    ui->FrameEvent->setVisible(false);                              // Hide the event frame, used while the cinescenie is running
    ui->LabelCinescenieName->setText(filename);                     // Set the filename of the current cinescenie
    ui->ButtonStart->setDisabled(ui->TableEvents->rowCount() == 0); // Forbid to start the cinescenie if the event table is empty

    return true;
}

void ExecCinescenie::buttonCloseClicked()
{
    // Confirmation needed if a cinescenie is running
    if (this->Started) {
        if (QMessageBox::question(this,
                                  tr("Cinéscénie en cours !"),
                                  tr("Voulez-vous vraiment quitter pendant qu'une cinéscénie est en cours ?")) == QMessageBox::No) {
            return;
        }
    }

    emit execCinescenieClosed();
}

void ExecCinescenie::buttonStartClicked()
{
    // First of all, initialize a QTime with current time, to measure elapsed time from start
    this->StartTime.start();

    // Start a timer to refresh UI
    startTimer(INTERVAL_UI_TIMER, Qt::CoarseTimer);

    // Discard events whose timecode < timecode preset. We need that because:
    // - triggers could happen at start if we compare timecodes with <=
    // - triggers could be skipped in case of lag, if we compare timecodes with ==
    // So we compare them with <=, but discard at start the ones which are "in the past"
    while (nvmEvent(0)->timecode() < ui->EditTimecode->time()) {
        delete nvmEvent(0);
        ui->TableEvents->removeRow(0);
    }

    // Safety check: if all events have been removed, we can't continue
    if (ui->TableEvents->rowCount() == 0) {
        return;
    }

    // UI
    ui->FrameDefineTimecode->setVisible(false);
    ui->FrameEvent->setVisible(true);

    // UI settings. Use the first event to initialize display
    ui->LabelNextEventName->setText(nvmEvent(0)->message());
    ui->LabelCurrentTimecode->setText(ui->EditTimecode->time().toString());
}

void ExecCinescenie::timerEvent(QTimerEvent* event)
{
    (void)event;
    // TODO: 200ms event received
}
