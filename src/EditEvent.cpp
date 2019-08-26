#include "EditEvent.hpp"
#include "AnnouncementSound.hpp"
#include "ui_EditEvent.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSpinBox>

//  EditEvent
//
// Basic settings...
//
EditEvent::EditEvent(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::EditEvent)
    , EventModified(false)
{
    ui->setupUi(this);

    // Dialog buttons
    connect(ui->ButtonOK, &QPushButton::clicked, [this]() { accept(); });
    connect(ui->ButtonCancel, &QPushButton::clicked, [this]() { reject(); });

    // Remove question mark from title bar
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    // Fill the combo box with available sounds
    ui->ComboSound->addItems(AnnouncementSound::instance()->soundNames());

    // Hide the missing sound warning message
    ui->LabelSoundNotExisting->setVisible(false);

    // Connect announcements list buttons
    connect(ui->ButtonAddAnnouncement, &QPushButton::clicked, [this]() { addAnnouncementClicked(); });
    connect(ui->ButtonRemoveAnnouncement, &QPushButton::clicked, [this]() { removeAnnouncementClicked(); });

    // Connect announcements list widget
    connect(ui->ListAnnouncement, &QListWidget::itemSelectionChanged, [this] { listSelectionChanged(); });
}

//  EditEvent
//
// Fill dialog with event data
//
EditEvent::EditEvent(QWidget* parent, Event* event)
    : EditEvent(parent)
{
    // Fill the UI
    ui->LineeditEvent->setText(event->message());
    ui->EditTimeCode->setTime(event->timecode());
    if (event->hasAnnounce()) {
        // Enable the sound box
        ui->BoxAnnouncement->setChecked(true);

        // Set the final countdown value
        ui->SpinboxFinalCountdown->setValue(event->countdown());

        // Add the announcements in the list
        QList<QTime> list = event->announcements();
        for (int i = 0; i < list.size(); i++) {
            ui->ListAnnouncement->addItem(list.at(i).toString());
        }

        // If the announcement sound file doesn't exist, add it to the list, and make the warning message visible
        if (!AnnouncementSound::instance()->exists(event->sound())) {
            ui->ComboSound->addItem(event->sound());
            ui->LabelSoundNotExisting->setVisible(true);
        }
        ui->ComboSound->setCurrentText(event->sound());
    }

    // Connections needed to know if an event changed during edition
    // Announcement list modifications are notified when the Add/Delete buttons are pressed
    connect(ui->LineeditEvent, &QLineEdit::textEdited, [this]() { this->EventModified = true; });
    connect(ui->EditTimeCode, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(
        ui->ComboSound, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), [this]() { comboTextChanged(); });
    connect(
        ui->SpinboxFinalCountdown, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this]() { this->EventModified = true; });
}

EditEvent::~EditEvent()
{
    delete ui;
}

//  createEvent
//
// Create a new event
// Return a pointer to the event if the dialog was accepted, else nullptr
//
Event* EditEvent::createEvent(QWidget* parent)
{
    Event* event = nullptr;
    EditEvent editor(parent);
    if (editor.exec() == QDialog::Accepted) {
        event = new Event;
        editor.readEventFromUI(event);
    }
    return event;
}

//  editEvent
//
// Edit an event and save data on user validation
// Return true if the event was modified
//
bool EditEvent::editEvent(QWidget* parent, Event* event)
{
    EditEvent editor(parent, event);

    // Save new event values if the dialog was accepted
    if (editor.exec() == QDialog::Accepted) {
        editor.readEventFromUI(event);
    }

    return editor.EventModified;
}

//  readEventFromUI
//
// Grab data from UI to fill an event object
//
void EditEvent::readEventFromUI(Event* event)
{
    // If the announcement box is unchecked, don't save any sound information
    if (!ui->BoxAnnouncement->isChecked()) {
        event->setData(ui->LineeditEvent->text(), ui->EditTimeCode->time());
    }

    // Else save all sound infos
    else {
        // Build the QTime<QList> object with the announcement time list
        QList<QTime> announcements;
        for (int i = 0; i < ui->ListAnnouncement->count(); i++) {
            announcements << QTime::fromString(ui->ListAnnouncement->item(i)->text());
        }

        event->setData(ui->LineeditEvent->text(),
                       ui->EditTimeCode->time(),
                       ui->ComboSound->currentText(),
                       ui->SpinboxFinalCountdown->value(),
                       announcements);
    }
}

//  comboTextChanged
//
// Called when the sound combo changes. Alert the user if the selected sound file doesn't exist
//
void EditEvent::comboTextChanged()
{
    this->EventModified = true;
    bool exist          = AnnouncementSound::instance()->exists(ui->ComboSound->currentText());
    ui->LabelSoundNotExisting->setVisible(!exist);
}

void EditEvent::addAnnouncementClicked()
{
    QListWidgetItem* item = new QListWidgetItem(ui->EditAnnouncementTime->time().toString());
    ui->ListAnnouncement->addItem(item);
    this->EventModified = true;
}

void EditEvent::removeAnnouncementClicked()
{
    delete ui->ListAnnouncement->selectedItems().at(0);
    this->EventModified = true;
}

// Disable remove button if no selection
void EditEvent::listSelectionChanged()
{
    ui->ButtonRemoveAnnouncement->setEnabled(!ui->ListAnnouncement->selectedItems().isEmpty());
}
