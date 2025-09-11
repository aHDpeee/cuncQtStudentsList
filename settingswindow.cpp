#include "settingswindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QDebug>

SettingsWindow::SettingsWindow(QWidget* parent)
    : QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(">_ Settings");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* classLayout = new QHBoxLayout();
    classLayout->addWidget(new QLabel("Class Name:"));
    classNameEdit = new QLineEdit();
    classLayout->addWidget(classNameEdit);
    mainLayout->addLayout(classLayout);

    QHBoxLayout* lessonsLayout = new QHBoxLayout();
    lessonsLayout->addWidget(new QLabel("Lessons per day:"));
    for(int i=0;i<6;i++){
        lessonsSpin[i] = new QSpinBox();
        lessonsSpin[i]->setRange(0,12);
        lessonsSpin[i]->setPrefix(QString("Day%1: ").arg(i+1));
        lessonsLayout->addWidget(lessonsSpin[i]);
    }
    mainLayout->addLayout(lessonsLayout);

    snackOnLunchCheck = new QCheckBox("Always snack on lunch if lessons > 6 (city students)");
    mainLayout->addWidget(snackOnLunchCheck);

    QHBoxLayout* colorLayout = new QHBoxLayout();
    bgColorButton = new QPushButton("Background Color");
    textColorButton = new QPushButton("Text Color");
    colorLayout->addWidget(bgColorButton);
    colorLayout->addWidget(textColorButton);
    mainLayout->addLayout(colorLayout);

    fontCombo = new QFontComboBox();
    mainLayout->addWidget(new QLabel("Font:"));
    mainLayout->addWidget(fontCombo);

    QHBoxLayout* botLayout = new QHBoxLayout();
    botLayout->addWidget(new QLabel("Bot Token:"));
    botTokenEdit = new QLineEdit();
    botLayout->addWidget(botTokenEdit);
    mainLayout->addLayout(botLayout);

    QHBoxLayout* chatLayout = new QHBoxLayout();
    chatLayout->addWidget(new QLabel("Chat ID:"));
    chatIdEdit = new QLineEdit();
    chatLayout->addWidget(chatIdEdit);
    mainLayout->addLayout(chatLayout);

    saveButton = new QPushButton("Save");
    mainLayout->addWidget(saveButton);

    connect(bgColorButton, &QPushButton::clicked, this, &SettingsWindow::chooseBgColor);
    connect(textColorButton, &QPushButton::clicked, this, &SettingsWindow::chooseTextColor);
    connect(saveButton, &QPushButton::clicked, this, &SettingsWindow::saveClicked);

    loadSettings();
}

void SettingsWindow::loadSettings(){
    QSettings s("config.ini", QSettings::IniFormat);
    classNameEdit->setText(s.value("ClassName","11 ИТ").toString());
    for(int i=0;i<6;i++) lessonsSpin[i]->setValue(s.value(QString("Lesson%1").arg(i),6).toInt());
    snackOnLunchCheck->setChecked(s.value("SnackOnLunch",false).toBool());
    bgColor = s.value("BgColor","#000000").value<QColor>();
    textColor = s.value("TextColor","#FFFFFF").value<QColor>();
    fontCombo->setCurrentFont(QFont(s.value("Font","Arial").toString()));
    botTokenEdit->setText(s.value("BotToken","").toString());
    chatIdEdit->setText(s.value("ChatID","").toString());
}

void SettingsWindow::saveSettings(){
    QSettings s("config.ini", QSettings::IniFormat);
    s.setValue("ClassName", classNameEdit->text());
    for(int i=0;i<6;i++) s.setValue(QString("Lesson%1").arg(i), lessonsSpin[i]->value());
    s.setValue("SnackOnLunch", snackOnLunchCheck->isChecked());
    s.setValue("BgColor", bgColor.name());
    s.setValue("TextColor", textColor.name());
    s.setValue("Font", fontCombo->currentFont().family());
    s.setValue("BotToken", botTokenEdit->text());
    s.setValue("ChatID", chatIdEdit->text());
    s.sync();
}

void SettingsWindow::chooseBgColor(){
    QColor color = QColorDialog::getColor(bgColor,this,"Choose Background Color");
    if(color.isValid()) bgColor = color;
}

void SettingsWindow::chooseTextColor(){
    QColor color = QColorDialog::getColor(textColor,this,"Choose Text Color");
    if(color.isValid()) textColor = color;
}

void SettingsWindow::saveClicked(){
    saveSettings();
    emit settingsSaved();
    close();
}
