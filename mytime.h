#ifndef MYTIME_H
#define MYTIME_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class MyTime : public QWidget
{
    Q_OBJECT
public:
    explicit MyTime(QWidget *parent = nullptr);
    ~MyTime();
    QLabel* time;
    QTimer t;
private:
    void resizeEvent(QResizeEvent* event) override;
    void savePos();
    void loadPos();

};

#endif // MYTIME_H
