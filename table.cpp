#include "table.h"
#include <algorithm>
#include "settingswindow.h"
#include "TelegramSenderWidget.h"
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>
#include <QSettings>
#include <QLineEdit>
#include <QDate>
#include "qrcodewindow.h"
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QSpacerItem>
#include <QPalette>
#include <QFont>

Table::Table(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(windowFlags() | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    qDebug()<<"Table";

    lay = new QVBoxLayout(this);
    studentslay = new QVBoxLayout();
    lay->setSpacing(5);
    lay->setContentsMargins(5,5,5,5);

    newStd = new QLineEdit();
    newStd->setPlaceholderText("Фамилия Имя");

    settingsWindow = new SettingsWindow();
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(settingsWindow, &SettingsWindow::settingsSaved, [this](){
        loadSettings();
        loadData();
    });

    QSettings s("config.ini", QSettings::IniFormat);
    className = s.value("ClassName","11 ИТ").toString();
    for(int i=0;i<6;i++) lessons[i] = s.value(QString("Lesson%1").arg(i),6).toInt();
    snackOnLunch = s.value("SnackOnLunch",false).toBool();
    QString bg = s.value("BgColor","#000000").toString();
    QString fg = s.value("TextColor","#FFFFFF").toString();
    QFont f(s.value("Font","Arial").toString());
    botToken = s.value("BotToken","").toString();
    chatId = s.value("ChatID","").toString();

    QPalette p = palette();
    p.setColor(QPalette::Window,QColor(bg));
    p.setColor(QPalette::WindowText,QColor(fg));
    setPalette(p);
    setFont(f);
    setAutoFillBackground(true);

    qr = new QrCodeWindow();
    qr->show();
    connect(this, &Table::makeQrCode, qr, &QrCodeWindow::update);

    loadData();
    loadPos();

    for (Student* s : students) {
        s->setAttribute(Qt::WA_DeleteOnClose);
        s->setMouseTracking(true);
        s->setAcceptDrops(true);
        s->installEventFilter(this);
    }

    lay->addLayout(studentslay);
    for (Student* s : students) studentslay->addWidget(s);
    lay->addWidget(newStd);

    connect(newStd, &QLineEdit::editingFinished, [this](){
        QString text = newStd->text().trimmed();
        if (text.isEmpty()) {
            return;
        }
        if(text == "set"){
            settingsWindow->show();
            return;
        }
        if(text == "t"){
            TelegramSenderWidget* t= new TelegramSenderWidget(botToken, chatId);
            t->show();
            return;

        }
        for (Student* ss : students) if (ss->fio == text) return;

        Student* s1 = new Student(text);
        students.push_back(s1);
        s1->setAttribute(Qt::WA_DeleteOnClose);
        s1->setMouseTracking(true);
        s1->setAcceptDrops(true);
        s1->installEventFilter(this);
        connect(s1, &Student::reqDelMe, this, Table::delStudent);
        connect(s1, &Student::dataChanged, this, Table::qrCodeUpdate);

        std::sort(students.begin(), students.end(), [](Student* a, Student* b){
            return a->fio.toLower() < b->fio.toLower();
        });
        for(int i=0;i<studentslay->count()-1;i++) studentslay->removeItem(studentslay->itemAt(i));
        for(Student* s: students) studentslay->insertWidget(studentslay->count()-1,s);

        newStd->clear();
        qrCodeUpdate();
    });
}

Table::~Table() {
    saveData();
    savePos();
}

void Table::loadSettings(){
    QSettings s("config.ini", QSettings::IniFormat);
    className = s.value("ClassName","11 ИТ").toString();
    for(int i=0;i<6;i++) lessons[i] = s.value(QString("Lesson%1").arg(i),6).toInt();
    snackOnLunch = s.value("SnackOnLunch",false).toBool();
    QString bg = s.value("BgColor","#000000").toString();
    QString fg = s.value("TextColor","#FFFFFF").toString();
    QFont f(s.value("Font","Arial").toString());
    botToken = s.value("BotToken","").toString();
    chatId = s.value("ChatID","").toString();
    QPalette p = palette();
    p.setColor(QPalette::Window,QColor(bg));
    p.setColor(QPalette::WindowText,QColor(fg));
    setPalette(p);
    setFont(f);
    setAutoFillBackground(true);
}

void Table::delStudent(Student *s){
    students.removeOne(s);
    studentslay->removeWidget(s);
    s->deleteLater();
    adjustSize();
    saveData();
    qrCodeUpdate();
}

void Table::qrCodeUpdate(){
    int inter=0, gorod=0, all=0;
    for(Student* s: students){
        all++;
        if (s->here){
            if (s->internat) inter++; else gorod++;
        }
    }

    int today = QDate::currentDate().dayOfWeek();
    int breakTwo = inter + gorod,
        lunch = inter + ((lessons[today-1] >= 6 || snackOnLunch) ? gorod : 0),
        snacklunch = (lessons[today-1] >=6 && lessons[today-1]<8 ? gorod : 0),
        snack = inter +  (lessons[today-1] >= 8 ? gorod : 0);

    emit makeQrCode(QString(
                        "%1 %2/%3\n"
                        "Второй завтрак %4\n"
                        "Обед %5 Полдник на обеде %6\n"
                        "Полдник %7\n"
                        ).arg(className).arg(gorod+inter).arg(all).arg(breakTwo).arg(lunch).arg(snacklunch).arg(snack));
}

void Table::loadData() {
    students.clear();
    QLayoutItem *child;
    while ((child = studentslay->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
    QSettings s("config.ini", QSettings::IniFormat);
    s.beginGroup("Students");
    QStringList keys = s.childGroups();

    QList<Student*> loadedStudents;
    for (const QString& key : keys) {
        s.beginGroup(key);
        Student* student = new Student(
            s.value("FIO").toString(),
            s.value("Here").toBool(),
            s.value("Internat").toBool(),
            s.value("Group").toInt()
            );
        s.endGroup();
        loadedStudents.push_back(student);
        connect(student, &Student::reqDelMe, this, Table::delStudent);
        connect(student, &Student::dataChanged, this, Table::qrCodeUpdate);
        student->setAttribute(Qt::WA_DeleteOnClose);
        student->setMouseTracking(true);
        student->setAcceptDrops(true);
        student->installEventFilter(this);
    }
    s.endGroup();

    std::sort(loadedStudents.begin(), loadedStudents.end(), [](Student* a, Student* b){
        return a->fio.toLower() < b->fio.toLower();
    });

    students = loadedStudents;

    lay->addLayout(studentslay);
    for (Student* s : students) studentslay->addWidget(s);
    lay->addWidget(newStd);
    qrCodeUpdate();
}

void Table::saveData() {
    QSettings s("config.ini", QSettings::IniFormat);
    s.beginGroup("Students");
    int i = 0;
    for (Student* student : students) {
        s.beginGroup(QString("Student%1").arg(i));
        s.setValue("FIO", student->fio);
        s.setValue("Group", student->group);
        s.setValue("Here", student->here);
        s.setValue("Internat", student->internat);
        s.endGroup();
        ++i;
    }
    s.endGroup();
    s.sync();
}

void Table::savePos(){
    QSettings s("tablepos.ini", QSettings::IniFormat);
    s.setValue("x", geometry().x());
    s.setValue("y", geometry().y());
    s.sync();
}

void Table::loadPos(){
    QSettings s("tablepos.ini", QSettings::IniFormat);
    setGeometry(s.value("x").toInt(), s.value("y").toInt(), geometry().width(), geometry().height());
    adjustSize();
}

bool Table::eventFilter(QObject* obj, QEvent* event){
    Student* s = qobject_cast<Student*>(obj);
    if(!s) return false;

    static QPoint dragStartPos;

    if(event->type() == QEvent::MouseButtonPress){
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        if(e->button() == Qt::LeftButton) dragStartPos = e->pos();
    } else if(event->type() == QEvent::MouseMove){
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        if(!(e->buttons() & Qt::LeftButton)) return false;
        if((e->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance()) return false;

        QDrag* drag = new QDrag(s);
        QMimeData* mime = new QMimeData;
        mime->setText(s->fio);
        drag->setMimeData(mime);
        drag->exec(Qt::MoveAction);
        return true;
    } else if(event->type() == QEvent::DragEnter){
        QDragEnterEvent* e = static_cast<QDragEnterEvent*>(event);
        e->acceptProposedAction();
        return true;
    } else if(event->type() == QEvent::Drop){
        QDropEvent* e = static_cast<QDropEvent*>(event);
        int dropIndex = studentslay->indexOf(s);
        QString text = e->mimeData()->text();
        Student* dragged = nullptr;
        for(Student* st : students){
            if(st->fio == text){
                dragged = st;
                break;
            }
        }
        if(dragged && dragged != s){
            int oldIndex = studentslay->indexOf(dragged);
            studentslay->removeWidget(dragged);
            studentslay->insertWidget(dropIndex, dragged);
            students.removeOne(dragged);
            students.insert(dropIndex, dragged);
            qrCodeUpdate();
        }
        e->acceptProposedAction();
        return true;
    }
    return false;
}
