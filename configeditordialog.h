#ifndef CONFIGEDITORDIALOG_H
#define CONFIGEDITORDIALOG_H

#include <QDialog>

namespace Ui {
    class ConfigEditorDialog;
}

class ConfigEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigEditorDialog(const QString &configPath, QWidget *parent = nullptr);
    ~ConfigEditorDialog();

    private slots:
        void on_buttonBox_accepted();

private:
    Ui::ConfigEditorDialog *ui;
    QString m_configPath;
};

#endif // CONFIGEDITORDIALOG_H