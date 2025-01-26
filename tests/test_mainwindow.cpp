// tests/test_mainwindow.cpp
#include <QtTest>
#include "mainwindow.h"
#include <QPushButton>
#include <QTextEdit>
#include <QStatusBar>
#include <QProcess>
#include <QSignalSpy>

class TestMainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testInitialization();
    void testAppendLog();
    void testInstallPrereqButton();
    void testReloadConfigButton();
    void testProcessSignals();
};

void TestMainWindow::initTestCase()
{
    // Инициализация перед запуском тестов
}

void TestMainWindow::cleanupTestCase()
{
    // Очистка после выполнения тестов
}

void TestMainWindow::testInitialization()
{
    MainWindow mainWindow;
    QVERIFY(!mainWindow.isVisible());

    // Проверка наличия необходимых UI элементов
    QVERIFY(mainWindow.findChild<QPushButton*>("btnInstallPrereq"));
    QVERIFY(mainWindow.findChild<QPushButton*>("btnInstallBin"));
    QVERIFY(mainWindow.findChild<QPushButton*>("btnReloadConfig"));
    QVERIFY(mainWindow.findChild<QPushButton*>("btnEditConfig"));
    QVERIFY(mainWindow.findChild<QTextEdit*>("txtLog"));
    QVERIFY(mainWindow.statusBar());
}

void TestMainWindow::testAppendLog()
{
    MainWindow mainWindow;
    QString testMessage = "Тестовое сообщение для лога.";

    mainWindow.appendLog(testMessage);

    QTextEdit* log = mainWindow.findChild<QTextEdit*>("txtLog");
    QVERIFY(log != nullptr);

    QString logContent = log->toPlainText();
    QVERIFY(logContent.contains(testMessage));
}

void TestMainWindow::testInstallPrereqButton()
{
    MainWindow mainWindow;
    mainWindow.show();

    QPushButton* btnInstallPrereq = mainWindow.findChild<QPushButton*>("btnInstallPrereq");
    QVERIFY(btnInstallPrereq != nullptr);

    // Используем QSignalSpy для отслеживания сигнала appendLog
    QSignalSpy spy(&mainWindow, SIGNAL(appendLog(QString)));

    // Симулируем нажатие кнопки
    QTest::mouseClick(btnInstallPrereq, Qt::LeftButton);

    // Проверяем, что сигнал был вызван
    QCOMPARE(spy.count(), 1);

    // Дополнительные проверки можно добавить здесь
}

void TestMainWindow::testReloadConfigButton()
{
    MainWindow mainWindow;
    mainWindow.show();

    QPushButton* btnReloadConfig = mainWindow.findChild<QPushButton*>("btnReloadConfig");
    QVERIFY(btnReloadConfig != nullptr);

    // Создаем временный конфигурационный файл для тестирования
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString configPath = appDir.filePath("zapret/config.default");

    QFile configFile(configPath);
    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&configFile);
        out << "test_key=test_value\n";
        configFile.close();
    } else {
        QFAIL("Не удалось создать временный конфигурационный файл.");
    }

    // Используем QSignalSpy для отслеживания сигнала appendLog
    QSignalSpy spy(&mainWindow, SIGNAL(appendLog(QString)));

    // Симулируем нажатие кнопки
    QTest::mouseClick(btnReloadConfig, Qt::LeftButton);

    // Проверяем, что сигнал был вызван
    QCOMPARE(spy.count(), 1);

    // Проверяем, что в лог добавлено содержимое конфигурационного файла
    QList<QVariant> arguments = spy.takeFirst();
    QString logMessage = arguments.at(0).toString();
    QVERIFY(logMessage.contains("test_key=test_value"));

    // Удаляем временный файл
    QFile::remove(configPath);
}

void TestMainWindow::testProcessSignals()
{
    MainWindow mainWindow;

    // Подключаем QSignalSpy к сигналам процесса
    QSignalSpy finishedSpy(mainWindow.process, &QProcess::finished);
    QSignalSpy outputSpy(mainWindow.process, &QProcess::readyReadStandardOutput);
    QSignalSpy errorSpy(mainWindow.process, &QProcess::readyReadStandardError);

    // Настраиваем тестовый скрипт
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    QString scriptPath = appDir.filePath("tests/test_script.bat");

    QFile scriptFile(scriptPath);
    if (scriptFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&scriptFile);
        out << "@echo off\n";
        out << "echo Test output\n";
        out << "exit /B 0\n";
        scriptFile.close();
    } else {
        QFAIL("Не удалось создать тестовый скрипт.");
    }

    // Устанавливаем путь к скрипту в MainWindow (предполагается, что это возможно)
    // Для этого может понадобиться добавить сеттер или использовать тестовый билд

    // Примерно так (если метод доступен):
    // mainWindow.setScriptPath(scriptPath);

    // Запускаем процесс
    mainWindow.process->start("cmd.exe", QStringList() << "/C" << scriptPath);

    // Ждем завершения процесса
    QVERIFY(mainWindow.process->waitForFinished());

    // Проверяем сигналы
    QCOMPARE(finishedSpy.count(), 1);
    QCOMPARE(outputSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);

    // Проверяем код завершения
    QList<QVariant> finishedArgs = finishedSpy.takeFirst();
    QCOMPARE(finishedArgs.at(0).toInt(), 0);
}

QTEST_MAIN(TestMainWindow)
#include "test_mainwindow.moc"