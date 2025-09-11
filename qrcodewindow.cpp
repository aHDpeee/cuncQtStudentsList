#include "qrcodewindow.h"
#include <QVBoxLayout>
#include <QSettings>

QrCodeWindow::QrCodeWindow()
{
    setWindowFlags(windowFlags() | Qt::Tool);
    qrLabel->setScaledContents(true);
    loadPos();
    qDebug()<<"qrload";
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("background-color: black; color: white;");
    setWindowTitle(">_ Meals");
}

void QrCodeWindow::savePos(){
    QSettings s("qrpos.ini", QSettings::IniFormat);
    s.setValue("x", geometry().x());
    s.setValue("y", geometry().y());
    s.setValue("w", geometry().width());
    s.setValue("h", geometry().height());
    s.sync();
}

void QrCodeWindow::loadPos(){
    QSettings s("qrpos.ini", QSettings::IniFormat);
    setGeometry(s.value("x", 100).toInt(),
                s.value("y", 100).toInt(),
                s.value("w", 100).toInt(),
                s.value("h", 100).toInt()
                );

}

QrCodeWindow::~QrCodeWindow(){
    savePos();
}
