// tests/test_configeditordialog.cpp
#include <QDialogButtonBox>
#include <QtTest>
#include <QPushButton>
#include "ConfigEditorDialog.h"

class TestConfigEditorDialog : public QObject
{
    Q_OBJECT

private slots:
    void testSaveConfig();
};

void TestConfigEditorDialog::testSaveConfig()
{
    QString testPath = "test_output.config";
    ConfigEditorDialog dialog(testPath);

    // Set the plain text using the public getter
    dialog.getPlainTextEditConfig()->setPlainText("Test Content");

    // Find the "OK" button within the dialog's button box
    QDialogButtonBox *buttonBox = dialog.findChild<QDialogButtonBox*>("buttonBox");
    QVERIFY(buttonBox != nullptr); // Ensure buttonBox is found

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(okButton != nullptr); // Ensure OK button is found

    // Simulate a click on the "OK" button
    QTest::mouseClick(okButton, Qt::LeftButton);

    // Verify that the config file was created and contains the correct content
    QFile file(testPath);
    QVERIFY(file.exists());

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        QCOMPARE(content, QString("Test Content"));
        file.close();
    } else {
        QFAIL("Failed to open the config file for reading.");
    }

    // Clean up
    file.remove();
}

QTEST_MAIN(TestConfigEditorDialog)
#include "test_configeditordialog.moc"
