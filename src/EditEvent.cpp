#include "EditEvent.hpp"
#include "AnnouncementSound.hpp"
#include "ui_EditEvent.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLineEdit>
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
    ui->LabelSoundNotExisting->setText("");
}

//  EditEvent
//
// Fill dialog with event data
//
EditEvent::EditEvent(QWidget* parent, Event* event)
    : EditEvent(parent)
{
    // Fill the UI
    ui->EditText->setText(event->message());
    ui->EditTimeCode->setTime(event->timecode());
    ui->TimeFirstAnnouncement->setTime(event->firstAnnouncementDelay());
    ui->TimeSecondAnnouncement->setTime(event->secondAnnouncementDelay());
    ui->TimeThrirdAnnouncement->setTime(event->thirdAnnouncementDelay());
    ui->SpinboxFinalCountdown->setValue(event->finalCountdownDelay());
    ui->CheckFirstAnnouncement->setChecked(event->firstAnnouncement());
    ui->CheckSecondAnnouncement->setChecked(event->secondAnnouncement());
    ui->CheckThirdAnnouncement->setChecked(event->thirdAnnouncement());
    ui->CheckFinalCountdown->setChecked(event->finalCountdown());

    // If the announcement sound file doesn't exist, add it to the list, and make the warning message visible
    if (!AnnouncementSound::instance()->exists(event->sound())) {
        ui->ComboSound->addItem(event->sound());
        ui->LabelSoundNotExisting->setText(tr("Attention, le son de cet énènement n'existe pas !"));
    }
    ui->ComboSound->setCurrentText(event->sound());

    // Connections needed to know if an event changed during edition
    connect(ui->EditText, &QLineEdit::textEdited, [this]() { this->EventModified = true; });
    connect(ui->EditTimeCode, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(ui->TimeFirstAnnouncement, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(ui->TimeSecondAnnouncement, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(ui->TimeThrirdAnnouncement, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(
        ui->SpinboxFinalCountdown, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this]() { this->EventModified = true; });
    connect(ui->CheckFirstAnnouncement, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
    connect(ui->CheckSecondAnnouncement, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
    connect(ui->CheckThirdAnnouncement, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
    connect(ui->CheckFinalCountdown, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
    connect(
        ui->ComboSound, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), [this]() { comboTextChanged(); });
}

EditEvent::~EditEvent()
{
    delete ui;
}

//  createEvent
//
// Create an event object and fill it it user validates the dialog
//
Event* EditEvent::createEvent(QWidget* parent)
{
    Event* event = nullptr;
    EditEvent editor(parent);
    if (editor.exec() == QDialog::Accepted) {
        event = new Event;
        editor.setEventValues(event);
    }
    return event;
}

//  editEvent
//
// Edit an event and save data on user validation
//
bool EditEvent::editEvent(QWidget* parent, Event* event)
{
    EditEvent editor(parent, event);
    if (editor.exec() == QDialog::Rejected) {
        return false;
    }

    editor.setEventValues(event); // Useless if the event wasn't modified, but I don't care
    return editor.EventModified;
}

//  setEventvalue
//
// Grab data from UI to fill an event object
//
void EditEvent::setEventValues(Event* event)
{
    event->setData(ui->EditText->text(),
                   ui->EditTimeCode->time(),
                   ui->TimeFirstAnnouncement->time(),
                   ui->TimeSecondAnnouncement->time(),
                   ui->TimeThrirdAnnouncement->time(),
                   ui->SpinboxFinalCountdown->value(),
                   ui->CheckFirstAnnouncement->isChecked(),
                   ui->CheckSecondAnnouncement->isChecked(),
                   ui->CheckThirdAnnouncement->isChecked(),
                   ui->CheckFinalCountdown->isChecked(),
                   ui->ComboSound->currentText());
}

//  comboTextChanged
//
// Called when the sound combo changes. Alert the user if the selected sound file doesn't exist
//
void EditEvent::comboTextChanged()
{
    this->EventModified = true;
    QString text =
        AnnouncementSound::instance()->exists(ui->ComboSound->currentText()) ? "" : tr("Attention, le son de cet énènement n'existe pas !");
    ui->LabelSoundNotExisting->setText(text);
}
