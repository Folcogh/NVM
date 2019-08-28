#include "ExecCinescenie.hpp"
#include "Nvm.hpp"
#include "ui_ExecCinescenie.h"
#include <QCheckBox>
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
    connect(ui->CheckboxDisplayTimecode, &QCheckBox::toggled, [this]() { checkboxDisplayTimecodeToggled(); });
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

        // Create the widget for each cell
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

    killTimer(this->TimerID);
    this->Started = false;

    emit execCinescenieClosed();
}

void ExecCinescenie::buttonStartClicked()
{
    this->StartTime.start(); // Initialize a QTime with current time, to measure elapsed time from start
    this->TimerID = startTimer(INTERVAL_UI_TIMER, Qt::CoarseTimer); // Start a timer to refresh UI
    this->Started = true;                                           // We will ask a confirmation before exiting the cinescenie

    // Discard events whose timecode < timecode preset. We need that because:
    // - triggers could happen at start if we compare timecodes with <=
    // - triggers could be skipped in case of lag, if we compare timecodes with ==
    // So we compare them with <=, but discard at start the ones which are "in the past"
    while ((ui->TableEvents->rowCount() != 0) && (nvmEvent(0)->timecode() < ui->EditTimecode->time())) {
        delete nvmEvent(0);
        ui->TableEvents->removeRow(0);
    }

    // Safety check: if all events have been removed, we can't continue
    if (ui->TableEvents->rowCount() == 0) {
        killTimer(this->TimerID);
        this->Started = false;
        return;
    }

    // UI settings. Use the first event to initialize display
    ui->ButtonStart->setEnabled(false);

    // Save the initial Time Code, to compute the current one in the timer event handler
    this->InitialTimecode = ui->EditTimecode->time();
}

void ExecCinescenie::timerEvent(QTimerEvent*)
{
    // Update current timecode
    QTime CurrentTC = this->InitialTimecode.addMSecs(this->StartTime.elapsed());
    ui->EditTimecode->setTime(CurrentTC);

    // Update remaining time for each event
    // If an event is done, register it for removal
    int ToBeRemoved = 0;
    for (int row = 0; row < ui->TableEvents->rowCount(); row++) {
        int seconds = CurrentTC.secsTo(nvmEvent(row)->timecode());
        int h       = seconds / 3600;
        int m       = (seconds % 3600) / 60;
        int s       = (seconds % 3600) % 60;
        QTime remaining(h, m, s);

        // Display remaining time if it's valid, else prepare event for removal
        if (remaining.isValid()) {
            ui->TableEvents->item(row, COLUMN_REMAINING)->setText(remaining.toString());
        }
        else {
            ToBeRemoved++;
        }
    }

    // Remove done events
    for (; ToBeRemoved > 0; ToBeRemoved--) {
        delete nvmEvent(0);
        ui->TableEvents->removeRow(0);
    }

    // Stop cinescenie execution if all events are in the past
    if (ui->TableEvents->rowCount() == 0) {
        killTimer(this->TimerID);
        this->Started = false;
    }
}

void ExecCinescenie::checkboxDisplayTimecodeToggled()
{
    if (ui->CheckboxDisplayTimecode->isChecked()) {
        ui->TableEvents->showColumn(COLUMN_TIMECODE);
    }
    else {
        ui->TableEvents->hideColumn(COLUMN_TIMECODE);
    }
}
