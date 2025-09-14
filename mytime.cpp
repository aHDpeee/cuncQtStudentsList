#include "mytime.h"
#include <QVBoxLayout>
#include <QTime>
#include <cmath>
#include <QSettings>

MyTime::MyTime(QWidget *parent)
    : QWidget{parent}
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool);
    setStyleSheet("background-color: black; color: white;");
    setAttribute(Qt::WA_DeleteOnClose);
    loadPos();
    qDebug() << "QrLoaded";

    time = new QLabel(this);
    time->setText("888");
    time->setAlignment(Qt::AlignCenter);
    setWindowTitle(">_ Time");

    t.singleShot(1000-QTime::currentTime().msec(), [this](){t.start(100);});
    connect(&t, QTimer::timeout, [this](){
        time->setText(QTime::currentTime().toString("hh:mm:ss"));
    });


}


void MyTime::savePos(){
    QSettings s("timepos.ini", QSettings::IniFormat);
    s.setValue("x", geometry().x());
    s.setValue("y", geometry().y());
    s.setValue("w", geometry().width());
    s.setValue("h", geometry().height());
    s.sync();
}

void MyTime::loadPos(){
    QSettings s("timepos.ini", QSettings::IniFormat);
    setGeometry(s.value("x", 100).toInt(),
                s.value("y", 100).toInt(),
                s.value("w", 100).toInt(),
                s.value("h", 100).toInt()
                );

}

MyTime::~MyTime(){
    savePos();
}
