#include "configeditordialog.h"
#include "ui_configeditordialog.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

ConfigEditorDialog::ConfigEditorDialog(const QString &configPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigEditorDialog),
    m_configPath(configPath)
{
    ui->setupUi(this);

    // Загружаем содержимое конфигурационного файла
    QFile configFile(m_configPath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл конфигурации: " + m_configPath);
        reject();
        return;
    }

    QTextStream in(&configFile);
    QString content = in.readAll();
    configFile.close();

    ui->plainTextEditConfig->setPlainText(content);
}

ConfigEditorDialog::~ConfigEditorDialog()
{
    delete ui;
}

// Слот для обработки нажатия кнопки "OK"
void ConfigEditorDialog::on_buttonBox_accepted()
{
    QString newContent = ui->plainTextEditConfig->toPlainText();

    // Сохраняем изменения в файл конфигурации
    QFile configFile(m_configPath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи: " + m_configPath);
        return;
    }

    QTextStream out(&configFile);
    out << newContent;
    configFile.close();

    accept();
}