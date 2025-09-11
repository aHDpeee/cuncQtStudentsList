#ifndef STUDENT_H
#define STUDENT_H

#include <QLabel>
#include <QMouseEvent>
#include <QMenu>

class Student : public QLabel
{
    Q_OBJECT
public:
    Student(QString fio1);
    Student(QString fio1, bool here1, bool internat1, int group1);
    QString fio;
    bool here;
    bool internat;
    int group;

    void setGroup(int g);
protected:
    void init();
signals:
    void reqDelMe(Student* s);
    void dataChanged();

};

#endif // STUDENT_H
