#ifndef CONFIGEDITORDIALOG_H
#define CONFIGEDITORDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QString>

namespace Ui {
    class ConfigEditorDialog;
}

class ConfigEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigEditorDialog(const QString &configPath, QWidget *parent = nullptr);
    ~ConfigEditorDialog();
    QPlainTextEdit* getPlainTextEditConfig() const;
    private slots:
        void on_buttonBox_accepted();

public:
    Ui::ConfigEditorDialog *ui;
    QString m_configPath;
};

#endif // CONFIGEDITORDIALOG_H