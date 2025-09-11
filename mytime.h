#ifndef MYTIME_H
#define MYTIME_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QResizeEvent>

class MyTime : public QWidget
{
    Q_OBJECT
public:
    explicit MyTime(QWidget *parent = nullptr);
    ~MyTime();
    QLabel* time;
    QTimer t;
private:
void resizeEvent(QResizeEvent* event) override {
    int h = event->size().height();

    if (event->size().width()==h*6) return;
    resize(h*5, h);
    time->resize(h*5,h);
    time->setFont(QFont("Consolas", h*0.8));

    QWidget::resizeEvent(event);
}
    void savePos();
    void loadPos();

};

#endif // MYTIME_H
