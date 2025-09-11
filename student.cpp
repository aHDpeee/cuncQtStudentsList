#include "student.h"

#include <QMenu>

Student::Student(QString fio1) {
    setText(fio1);
    qDebug() << fio;
    fio = fio1;
    group = -1;
    internat = 0;
    here = 1;

    init();
}

Student::Student(QString fio1, bool here1, bool internat1, int group1){
    fio = fio1;
    here = here1;
    internat = internat1;
    group = group1;

    init();
}

void Student::setGroup(int g){
    group = g;
}

void Student::init(){
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QLabel::customContextMenuRequested, [this](const QPoint &pos){
            QMenu menu(this);
            menu.setStyleSheet("color:white; background: black;");
            menu.addAction("Присутствие", this, [this](){
                here = !here;
                if (here){
                    setStyleSheet("color: green;");
                } else {
                    setStyleSheet("color: red");
                }
                emit dataChanged();
            });
            menu.addAction("Интернат", this, [this](){
                internat = !internat;
                if (internat) setText(fio + "*");
                else setText(fio);
                emit dataChanged();
            });
            menu.addAction("Удалить", this, [this](){
                emit reqDelMe(this);

            });
            menu.exec(mapToGlobal(pos));

    });
    if (internat) setText(fio + "*");
    else setText(fio);
    if (here){
        setStyleSheet("color: green;");
    } else {
        setStyleSheet("color: red");
    }
}
