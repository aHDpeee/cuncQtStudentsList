#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include <QLineEdit>
#include <QSettings>
#include <QVBoxLayout>
#include <QEvent>
#include <QColor>
#include <QString>
#include <student.h>
#include "settingswindow.h"
#include "qrcodewindow.h"

class Table : public QWidget
{
    Q_OBJECT

public:
    Table(QWidget *parent = nullptr);
    ~Table();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QLineEdit* newStd;
    QVBoxLayout* lay;
    QVBoxLayout* studentslay;
    QList<Student*> students;
    int lessons[6] = {6,6,6,6,6,6};
    QString className = "11 ИТ";
    bool snackOnLunch = false;
    QString botToken;
    QString chatId;
    QColor bgColor;
    QColor textColor;
    QFont font;

    SettingsWindow* settingsWindow;
    QrCodeWindow* qr;

    void delStudent(Student* s);
    void loadData();
    void qrCodeUpdate();
    void saveData();
    void savePos();
    void loadPos();
    void loadSettings();

signals:
    void makeQrCode(QString data);
};

#endif // TABLE_H
