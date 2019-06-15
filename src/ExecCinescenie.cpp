#include "ExecCinescenie.hpp"
#include "Event.hpp"
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

    connect(ui->ButtonClose, &QPushButton::clicked, [this]() { buttonCloseClicked(); });
}

ExecCinescenie::~ExecCinescenie()
{
    deleteEvents();
    delete ui;
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

    // Display the timecode setting frame, mask the next event frame
    ui->FrameDefineTimecode->setVisible(true);
    ui->FrameEvent->setVisible(false);
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

void ExecCinescenie::deleteEvents()
{
    for (int row = 0; row < ui->TableEvents->rowCount(); row++) {
        delete ui->TableEvents->item(row, COLUMN_TIMECODE)->data(EVENT_ROLE).value<Event*>();
    }
}
