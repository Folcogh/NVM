#include "EditEvent.hpp"
#include "ui_EditEvent.h"
#include <QCheckBox>
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
}

//  EditEvent
//
// Fill dialog with event data
//
EditEvent::EditEvent(QWidget* parent, Event* event)
    : EditEvent(parent)
{
    ui->EditText->setText(event->message());
    ui->EditTimeCode->setTime(event->timecode());
    ui->TimeFirstAnnounce->setTime(event->firstAnnounceDelay());
    ui->TimeSecondAnnounce->setTime(event->secondAnnounceDelay());
    ui->TimeThrirdAnnounce->setTime(event->thirdAnnounceDelay());
    ui->SpinboxFinalCountdown->setValue(event->finalCountdownDelay());
    ui->CheckFirstAnnounce->setChecked(event->firstAnnounce());
    ui->CheckSecondAnnounce->setChecked(event->secondAnnounce());
    ui->CheckThirdAnnounce->setChecked(event->thirdAnnounce());
    ui->CheckFinalCountdown->setChecked(event->finalCountdown());

    // Connect inputs changes to the modified flags
    connect(ui->EditText, &QLineEdit::textEdited, [this]() { this->EventModified = true; });
    connect(ui->EditTimeCode, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(ui->TimeFirstAnnounce, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(ui->TimeSecondAnnounce, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(ui->TimeThrirdAnnounce, &QDateTimeEdit::timeChanged, [this]() { this->EventModified = true; });
    connect(
        ui->SpinboxFinalCountdown, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this]() { this->EventModified = true; });
    connect(ui->CheckFirstAnnounce, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
    connect(ui->CheckSecondAnnounce, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
    connect(ui->CheckThirdAnnounce, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
    connect(ui->CheckFinalCountdown, &QCheckBox::stateChanged, [this]() { this->EventModified = true; });
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
                   ui->TimeFirstAnnounce->time(),
                   ui->TimeSecondAnnounce->time(),
                   ui->TimeThrirdAnnounce->time(),
                   ui->SpinboxFinalCountdown->value(),
                   ui->CheckFirstAnnounce->isChecked(),
                   ui->CheckSecondAnnounce->isChecked(),
                   ui->CheckThirdAnnounce->isChecked(),
                   ui->CheckFinalCountdown->isChecked());
}
