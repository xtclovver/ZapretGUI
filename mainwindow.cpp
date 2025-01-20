#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>
#include <QFileDialog>
#include <QOperatingSystemVersion> // Include this header


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , process(new QProcess(this))
{
    ui->setupUi(this);

    // Connect process signals to slots
    connect(process, &QProcess::finished, this, &MainWindow::processFinished);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::processOutput);
    connect(process, &QProcess::readyReadStandardError, this, &MainWindow::processOutput);
}

// Деструктор главного окна
MainWindow::~MainWindow()
{
    delete ui;
}

// Функция для добавления сообщений в лог с таймстампом
void MainWindow::appendLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->txtLog->append(QString("[%1] %2").arg(timestamp, message));
}

void MainWindow::on_btnInstallPrereq_clicked()
{
    const QOperatingSystemVersion osVersion = QOperatingSystemVersion::current();
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp(); // Move to project directory
    QString scriptPath;

    if (osVersion.type() == QOperatingSystemVersion::Windows) {
        if (osVersion >= QOperatingSystemVersion::Windows10) {
            // Windows 10 or 11
            scriptPath = appDir.filePath("zapret-win\\zapret-winws\\service_create.cmd");
        } else if (osVersion <= QOperatingSystemVersion::Windows7) {
            // Windows 7
            scriptPath = appDir.filePath("zapret-win\\win7\\install_win7.cmd");
        } else {
            QMessageBox::warning(this, "Unsupported OS", "Unsupported Windows version.");
            return;
        }
    } else {
        QMessageBox::warning(this, "Unsupported OS", "Unsupported Operating System.");
        return;
    }

    appendLog("Launching prerequisite installation script: " + scriptPath);

    if (!QFile::exists(scriptPath)) {
        QMessageBox::critical(this, "Error", "Script file not found: " + scriptPath);
        return;
    }

    // Execute the script
    if (osVersion.type() == QOperatingSystemVersion::Windows) {
        process->start("cmd.exe", QStringList() << "/C" << scriptPath);
    }

    ui->statusbar->showMessage("Executing script...");
}

void MainWindow::on_btnInstallBin_clicked()
{
    QOperatingSystemVersion osVersion = QOperatingSystemVersion::current();
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp(); // Move to project directory
    QString appPath = appDir.absolutePath();
    QString currentPath = QDir::currentPath();

    appendLog("Application directory: " + appPath);
    appendLog("Current working directory: " + currentPath);

    QString scriptPath;

    if (osVersion.type() == QOperatingSystemVersion::Windows) {
        if (osVersion >= QOperatingSystemVersion::Windows10) {
            // Windows 10 or 11
            scriptPath = appDir.filePath("zapret-win\\zapret-winws\\service_create.cmd");
        } else if (osVersion <= QOperatingSystemVersion::Windows7) {
            // Windows 7
            scriptPath = appDir.filePath("zapret-win\\win7\\install_win7.cmd");
        } else {
            QMessageBox::warning(this, "Unsupported OS", "Unsupported Windows version.");
            return;
        }
    } else {
        QMessageBox::warning(this, "Unsupported OS", "Unsupported Operating System.");
        return;
    }

    appendLog("Constructed script path: " + scriptPath);

    QFileInfo fileInfo(scriptPath);
    appendLog("Script absolute path: " + fileInfo.absoluteFilePath());
    appendLog("Script exists: " + QString::number(fileInfo.exists()));

    if (!fileInfo.exists()) {
        // List files in the target directory
        QString targetDirPath = fileInfo.absolutePath();
        QDir targetDir(targetDirPath);
        appendLog("Listing files in directory: " + targetDirPath);
        QStringList files = targetDir.entryList(QDir::Files | QDir::NoSymLinks);
        for (const QString &file : files) {
            appendLog(" - " + file);
        }

        QMessageBox::critical(this, "Error", "Script file not found: " + scriptPath);
        return;
    }

    appendLog("Launching binary installation script: " + scriptPath);

    if (osVersion.type() == QOperatingSystemVersion::Windows) {
        QProcess *process = new QProcess(this);
        connect(process, &QProcess::readyRead, this, [this, process]() {
            QString output = process->readAll();
            appendLog(output);
        });
        connect(process, QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred), this, [this, process](QProcess::ProcessError error) {
            appendLog("Process error: " + QString::number(static_cast<int>(error)));
            QMessageBox::critical(this, "Process Error", "An error occurred while executing the script.");
        });
        process->start("cmd.exe", QStringList() << "/C" << scriptPath);
        if (!process->waitForStarted()) {
            appendLog("Failed to start the process.");
            QMessageBox::critical(this, "Execution Error", "Failed to start the script process.");
            return;
        }
    }

    ui->statusbar->showMessage("Executing script...");
}



// Слот для кнопки "Перезагрузить конфигурацию"
void MainWindow::on_btnReloadConfig_clicked()
{
    // Определяем путь к файлу config.default
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString configPath = appDir.filePath("zapret\\config.default");

    appendLog("Перезагрузка конфигурации: " + configPath);

    // Читаем содержимое файла конфигурации
    QFile configFile(configPath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        appendLog("Не удалось открыть файл конфигурации: " + configPath);
        return;
    }

    QTextStream in(&configFile);
    QString content = in.readAll();
    configFile.close();

    // Отобразим содержимое в логах
    appendLog("Содержимое конфигурационного файла:\n" + content);
    ui->statusbar->showMessage("Конфигурация загружена.");
}

// Слот для обработки завершения процесса
void MainWindow::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        appendLog("Процесс завершен успешно.");
        ui->statusbar->showMessage("Выполнено успешно.");
    } else {
        appendLog(QString("Процесс завершен с ошибкой. Код выхода: %1").arg(exitCode));
        ui->statusbar->showMessage("Выполнено с ошибкой.");
    }
}

// Слот для обработки вывода процесса
void MainWindow::processOutput()
{
    QString output = process->readAllStandardOutput();
    QString error = process->readAllStandardError();

    if (!output.isEmpty()) {
        appendLog(output.trimmed());
    }
    if (!error.isEmpty()) {
        appendLog("Ошибка: " + error.trimmed());
    }
}

// Слот для кнопки "Редактировать конфигурацию"
void MainWindow::on_btnEditConfig_clicked()
{
    // Определяем путь к файлу config.default
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString configPath = appDir.filePath("zapret\\config.default");

    if (!QFile::exists(configPath)) {
        QMessageBox::critical(this, "Ошибка", "Файл конфигурации не найден: " + configPath);
        return;
    }

    // Открываем диалоговое окно редактирования файла
    ConfigEditorDialog editorDialog(configPath, this);
    if (editorDialog.exec() == QDialog::Accepted) {
        appendLog("Конфигурация обновлена.");
        ui->statusbar->showMessage("Конфигурация сохранена.");
    }
}