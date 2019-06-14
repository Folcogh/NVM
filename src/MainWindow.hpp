#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "EditCinescenie.hpp"
#include "ExecCinescenie.hpp"
#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadCinescenie(QString filename);

private:
    // UI
    Ui::MainWindow* ui;

    // Central widgets
    EditCinescenie* WidgetEdit;
    ExecCinescenie* WidgetExec;

    // Default directory for file opening
    QString DefaultDirectory;

    // Methods
    QString requestCinescenieFile(QString title);
    void displayChoice();

    // Slots
public slots:
    void newCinescenieTriggered();
    void loadCinescenieTriggered();
    void modifyCinescenieTriggered();
};

#endif // MAINWINDOW_HPP
