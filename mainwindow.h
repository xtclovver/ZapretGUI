#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QOperatingSystemVersion>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Добавляем включение заголовочного файла диалога
#include "configeditordialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
    void on_btnInstallPrereq_clicked();
    void on_btnInstallBin_clicked();
    void on_btnReloadConfig_clicked();
    void on_btnEditConfig_clicked();

    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processOutput();

private:
    Ui::MainWindow *ui;
    QProcess *process;

    void appendLog(const QString &message);
};

#endif // MAINWINDOW_H