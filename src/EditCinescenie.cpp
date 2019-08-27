#include "EditCinescenie.hpp"
#include "EditEvent.hpp"
#include "Nvm.hpp"
#include "ui_EditCinescenie.h"
#include <QCheckBox>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidgetItem>

//  EditCinescenie
//
// Nothing special here, just button connections
//
EditCinescenie::EditCinescenie(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::EditCinescenie)
    , DefaultDirectory(QDir::homePath())
{
    ui->setupUi(this);

    // File handling
    connect(ui->ButtonSave, &QPushButton::clicked, [this]() { buttonSaveClicked(); });
    connect(ui->ButtonsSaveAs, &QPushButton::clicked, [this]() { buttonSaveAsClicked(); });
    connect(ui->ButtonClose, &QPushButton::clicked, [this]() { buttonCloseClicked(); });

    // Event edition
    connect(ui->ButtonAddEvent, &QPushButton::clicked, [this]() { buttonAddEventClicked(); });
    connect(ui->ButtonModifyEvent, &QPushButton::clicked, [this]() { buttonModifyEventClicked(); });
    connect(ui->ButtonDeleteEvent, &QPushButton::clicked, [this]() { buttonDeleteEventClicked(); });

    // Table events
    connect(ui->TableEvents, &QTableWidget::itemSelectionChanged, [this]() { updateUI(); });
    connect(ui->TableEvents, &QTableWidget::cellDoubleClicked, [this](int row, int column) { editEvent(row, column); });
}

//  ~EditCinescenie
//
// Delete Event ojbects associated with every row
//
EditCinescenie::~EditCinescenie()
{
    deleteEvents();
    delete ui;
}

//  editNewCinescenie
//
// Prepare editor to edit a new cinescenie
//
void EditCinescenie::editNewCinescenie()
{
    // Clear the table
    deleteEvents();
    ui->TableEvents->clearContents();
    ui->TableEvents->setRowCount(0);

    // Some settings
    this->Filename.clear();
    this->FileModified = false;
    updateUI();
}

//  editCinescenie
//
// Open a cinescenie file for edition and prepare editor
//
bool EditCinescenie::editCinescenie(QString filename)
{
    // Clear the table
    deleteEvents();
    ui->TableEvents->clearContents();
    ui->TableEvents->setRowCount(0);

    // Open the file and check for success
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("Erreur"), QString("Impossible d'ouvrir le fichier %1").arg(filename));
        return false;
    }

    // Create the associated stream
    QDataStream stream(&file);
    while (!stream.atEnd()) {
        // Create an event, fill it with stream data, and check for success
        Event* event = new Event;
        stream >> *event;
        if (stream.status() != QDataStream::Ok) {
            QMessageBox::critical(this, tr("Erreur"), QString("Erreur en lisant le fichier %1").arg(filename));
            delete event;
            return false;
        }

        // Add the event to the table
        addEvent(event);
    }

    this->Filename     = filename;
    this->FileModified = false;
    updateUI();
    return true;
}

//  buttonCloseClicked
//
// Called when the close button is clicked. Check if there is something to save before closing
//
void EditCinescenie::buttonCloseClicked()
{
    // Prompt a request for saving if the cinescenie was modified
    if (this->FileModified) {
        QMessageBox* box = new QMessageBox(QMessageBox::Question,
                                           tr("Cinéscénie modifiée"),
                                           tr("La cinéscénie a été modifiée. Voulez-vous l'enregistrer ?"),
                                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                           this);

        int ret = box->exec();

        // if the user wants to save, do it and close.
        // if the user doesn't want, just close
        // else cancel closing
        if (ret == QMessageBox::Yes) {
            if (this->Filename.isEmpty()) {
                // No filename defined, simulate a "Save As" procedure
                if (buttonSaveAsClicked()) {
                    emit editCinescenieClosed();
                }
            }
            else {
                // A filename exists, try to save directly
                if (buttonSaveClicked()) {
                    emit editCinescenieClosed();
                }
            }
        }
        if (ret == QMessageBox::No) {
            emit editCinescenieClosed();
        }
    }
    // The file is not moified, we can close
    else {
        emit editCinescenieClosed();
    }
}

//  buttonAddEventClicked
//
// Add an event to the table
//
void EditCinescenie::buttonAddEventClicked()
{
    // Prompt for a new event, quit if it was cancelled
    Event* event = EditEvent::createEvent(this);
    if (event == nullptr) {
        updateUI(); // Not sure it's usefull, but...
        return;
    }

    // Add the event to the table
    addEvent(event);

    // Update file status and UI
    this->FileModified = true;
    updateUI();
}

//  buttonModifyEventClicked
//
// Called when the user wants to modify the current event
//
void EditCinescenie::buttonModifyEventClicked()
{
    editEvent(ui->TableEvents->currentRow(), 0);
}

//  buttonDeleteEvent
//
// Called when the user wants to delete the current event
//
void EditCinescenie::buttonDeleteEventClicked()
{
    // Get the current row, delete the associated event, then remove the row
    int row = ui->TableEvents->currentRow();
    delete nvmEvent(row);
    ui->TableEvents->removeRow(row);

    this->FileModified = true;
    updateUI();
}

//  buttonSaveClicked
//
// Called when the user click the Save button
//
bool EditCinescenie::buttonSaveClicked()
{
    // Save the file
    bool ret = save(this->Filename);

    // Display an error message in case of error
    if (!ret) {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'enregistrer le fichier %1").arg(this->Filename));
    }

    // Set Modified flag according to save success
    this->FileModified = !ret;
    updateUI();
    return ret;
}

//  buttonSaveAsClicked
//
// Called when the user wants to save current cinescenie in an other file than the current one
//
bool EditCinescenie::buttonSaveAsClicked()
{
    // Get a new file name where the cinescenie should be saved. Quit if the user cancelled saving
    QString filename =
        QFileDialog::getSaveFileName(this, tr("Enregistrer la cinéscénie"), this->DefaultDirectory, "Cinéscénie (*." FILE_EXTENSION ")");
    if (filename.isEmpty()) {
        return false;
    }

    // Save the file, and set flags according to success
    bool ret = save(filename);
    if (ret) {
        this->FileModified     = false;
        this->Filename         = filename;
        this->DefaultDirectory = QFileInfo(filename).canonicalPath();
    }
    // Else display an error message
    else {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'enregistrer le fichier %1").arg(filename));
    }

    // Update UI and quit
    updateUI();
    return ret;
}

//  save
//
// Save the cinescenie in the file whose name is filename.
// Don't display any message if something goes wrong, just return false
//
bool EditCinescenie::save(QString filename)
{
    // Open the file in write only mode and return if it fails
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    // Create the associated stream
    QDataStream stream(&file);

    // Save all events
    for (int row = 0; row < ui->TableEvents->rowCount(); row++) {
        stream << *nvmEvent(row);

        // Check stream status
        if (stream.status() != QDataStream::Ok) {
            return false;
        }
    }

    // Success!
    return true;
}

//  event
//
// Return a pointer to the event of a given row. Inlined method
//
Event* EditCinescenie::nvmEvent(int row) const
{
    return ui->TableEvents->item(row, COLUMN_TIMECODE)->data(EVENT_ROLE).value<Event*>();
}

//  addEvent
//
// Add an event to the table, by increasing table size
//
void EditCinescenie::addEvent(Event* event)
{
    // Add the new row into the table
    int row = ui->TableEvents->rowCount();
    ui->TableEvents->setRowCount(row + 1);

    // Create items
    QTableWidgetItem* timecode     = new QTableWidgetItem;
    QTableWidgetItem* announcement = new QTableWidgetItem;
    QTableWidgetItem* text         = new QTableWidgetItem;

    // Configure items
    timecode->setData(Qt::UserRole, QVariant::fromValue(event));
    timecode->setTextAlignment(Qt::AlignCenter);
    announcement->setTextAlignment(Qt::AlignCenter);

    // Insert items in the table
    ui->TableEvents->setItem(row, COLUMN_TIMECODE, timecode);
    ui->TableEvents->setItem(row, COLUMN_ANNOUNCEMENT, announcement);
    ui->TableEvents->setItem(row, COLUMN_MESSAGE, text);

    // Update items
    updateEventDisplay(row);
}

//  editEvent
//
// Called when the user wants to modify an event, by double-clicking it or by clicking the "Modify event" button
//
void EditCinescenie::editEvent(int row, int)
{
    this->FileModified = EditEvent::editEvent(this, nvmEvent(row));
    updateEventDisplay(row);
    updateUI();
}

//  deleteEvents
//
// Delete all the Events associated with QTableWidgetItems data of the table
//
void EditCinescenie::deleteEvents()
{
    for (int row = 0; row < ui->TableEvents->rowCount(); row++) {
        delete nvmEvent(row);
    }
}

//  updateUI
//
// Set buttons, cinescenie filename and Modified status according to the current data state
// Also sort the table, depending on time code
//
void EditCinescenie::updateUI()
{
    // Sort the table, based on time code
    ui->TableEvents->sortItems(COLUMN_TIMECODE);

    // Update table buttons
    if (ui->TableEvents->selectedItems().isEmpty()) {
        ui->ButtonModifyEvent->setDisabled(true);
        ui->ButtonDeleteEvent->setDisabled(true);
    }
    else {
        ui->ButtonModifyEvent->setEnabled(true);
        ui->ButtonDeleteEvent->setEnabled(true);
    }

    // Update file buttons
    if (this->FileModified && !this->Filename.isEmpty()) {
        ui->ButtonSave->setEnabled(true);
    }
    else {
        ui->ButtonSave->setDisabled(true);
    }

    // Update filename
    QString text;
    if (this->Filename.isEmpty()) {
        text = tr("< Fichier non-enregistré >");
    }
    else {
        text = this->Filename;
    }

    if (this->FileModified) {
        text.append(tr(" < modifié >"));
    }

    ui->LabelName->setText(text);
}

//  updateEventDisplay
//
// Update the data displayed by an event, after creation or edition
//
void EditCinescenie::updateEventDisplay(int row)
{
    ui->TableEvents->item(row, COLUMN_TIMECODE)->setText(nvmEvent(row)->timecode().toString());
    ui->TableEvents->item(row, COLUMN_ANNOUNCEMENT)->setText(nvmEvent(row)->hasAnnounce() ? tr("oui") : tr("non"));
    ui->TableEvents->item(row, COLUMN_MESSAGE)->setText(nvmEvent(row)->message());
}
