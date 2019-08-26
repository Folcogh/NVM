#ifndef EDITEVENT_HPP
#define EDITEVENT_HPP

#include "Event.hpp"
#include <QDialog>

namespace Ui {
class EditEvent;
}

//  EditEvent
//
// This class is a dialog which allows to edit a new or an existing event
// Static methods are provided for convenience
//
class EditEvent : public QDialog
{
    Q_OBJECT

public:
    // Event creation/edition
    static Event* createEvent(QWidget* parent);
    static bool editEvent(QWidget* parent, Event* event);

private:
    Ui::EditEvent* ui;
    bool EventModified; // True if the event is modified during edition

    explicit EditEvent(QWidget* parent);      // Used to create a new event
    EditEvent(QWidget* parent, Event* event); // Used to edit an existing event
    ~EditEvent();

    // Widgets handling
    void comboTextChanged();
    void addAnnouncementClicked();
    void removeAnnouncementClicked();
    void listSelectionChanged();

    // Grab event data from the UI
    void readEventFromUI(Event* event);
};

#endif // EDITEVENT_HPP
