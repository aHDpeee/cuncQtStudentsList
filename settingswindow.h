#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QFontComboBox>

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    SettingsWindow(QWidget* parent = nullptr);

signals:
    void settingsSaved();

private:
    QSpinBox* lessonsSpin[6];
    QCheckBox* snackOnLunchCheck;
    QLineEdit* classNameEdit;
    QPushButton* bgColorButton;
    QPushButton* textColorButton;
    QFontComboBox* fontCombo;
    QLineEdit* botTokenEdit;
    QLineEdit* chatIdEdit;
    QPushButton* saveButton;

    QColor bgColor;
    QColor textColor;

    void loadSettings();
    void saveSettings();

private slots:
    void chooseBgColor();
    void chooseTextColor();
    void saveClicked();
};

#endif // SETTINGSWINDOW_H
