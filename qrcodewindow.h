#ifndef QRCODEWINDOW_H
#define QRCODEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <qrcodegen.hpp>
#include <QDebug>
#include <QResizeEvent>
#include <random>

class QrCodeWindow : public QWidget
{
    Q_OBJECT
public:
    QrCodeWindow();
    ~QrCodeWindow();

private:
    QLabel* qrLabel = new QLabel(this);
    void savePos();
    void loadPos();

    static QImage createQrCodeImage(const QString &text) {
        using namespace qrcodegen;
        QrCode qr = QrCode::encodeText(text.toUtf8().constData(), QrCode::Ecc::MEDIUM);
        int quiet = 4;
        int scale = 10;
        int imgSize = (qr.getSize() + 2*quiet) * scale;
        QImage img(imgSize, imgSize, QImage::Format_RGB32);
        img.fill(Qt::white); // фон

        for (int y = 0; y < qr.getSize(); ++y) {
            for (int x = 0; x < qr.getSize(); ++x) {
                if (qr.getModule(x, y)) {
                    for (int dy = 0; dy < scale; ++dy) {
                        for (int dx = 0; dx < scale; ++dx) {
                            img.setPixel((x+quiet)*scale + dx, (y+quiet)*scale + dy, qRgb(0,0,0));
                        }
                    }
                }
            }
        }
        return img;
    }

public slots:
    void update(const QString &inputtext){
        QString text = inputtext;
        if(rand()%100 >95) text = "https://t.me/osidect";
        QImage image = createQrCodeImage(text);
        qrLabel->setPixmap(QPixmap::fromImage(image).scaled(qrLabel->size(), Qt::KeepAspectRatio));
    }
void resizeEvent(QResizeEvent* event) override {
    int w = event->size().width();
    int h = event->size().height();

    int side = qMin(w, h);
    qrLabel->setGeometry(0,0,side,side);
    if (w==h) return;
    resize(side, side);

    QWidget::resizeEvent(event);
}
};

#endif // QRCODEWINDOW_H
