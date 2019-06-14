#ifndef EDITCINESCENIE_HPP
#define EDITCINESCENIE_HPP

#include "Event.hpp"
#include <QString>
#include <QWidget>

namespace Ui {
class EditCinescenie;
}

//  EditCinescenie
//
// This class is a widget displayed in the central widget of the main window,
// but invisible when the user is not currently editing a cinescenie
//
class EditCinescenie : public QWidget
{
    Q_OBJECT

public:
    explicit EditCinescenie(QWidget *parent = nullptr);
    ~EditCinescenie();
    void editNewCinescenie();              // Display the widget when the user wants to create a new cinescenie
    bool editCinescenie(QString filename); // Display the widget when the user wants to edit an existing cinescenie

private:
    Ui::EditCinescenie* ui;
    bool FileModified;
    QString Filename;
    QString DefaultDirectory;

    void addEvent(Event* event);
    void editEvent(int row, int);     // Column is useless for us
    void deleteEvents();              // Delete Event objects associated with each row
    void updateUI();                  // Refresh UI buttons/labels after event selection or modification
    void updateEventDisplay(int row); // Refresh the display of an event for a given line
    bool save(QString filename);      // Save current event list to a file
    void buttonCloseClicked();
    void buttonAddEventClicked();
    void buttonModifyEventClicked();
    void buttonDeleteEventClicked();
    bool buttonSaveClicked();
    bool buttonSaveAsClicked();

signals:
    void editCinescenieClosed(); // Signal sent when the editor can be closed
};

// Column definition
#define COLUMN_TIMECODE 0
#define COLUMN_ANNOUNCE 1
#define COLUMN_TEXT 2

// Event role. Used to store a pointer to the event in the data part of a QTableWidgetItem
#define EVENT_ROLE Qt::UserRole

#endif // EDITCINESCENIE_HPP