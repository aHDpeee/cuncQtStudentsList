// TelegramSenderWidget.cpp
#include "TelegramSenderWidget.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

TelegramSenderWidget::TelegramSenderWidget(const QString &token, const QString &chatId, QWidget *parent)
    : QWidget(parent), botToken(token), chatId(chatId)
{
    loadPos();
    setWindowFlags(windowFlags() | Qt::Tool);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(">_ Telegram");
    manager = new QNetworkAccessManager(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    textEdit = new QTextEdit();
    sendButton = new QPushButton("Отправить");

    textEdit->installEventFilter(this);
    layout->addWidget(textEdit);
    layout->addWidget(sendButton);

    connect(sendButton, &QPushButton::clicked, this, &TelegramSenderWidget::onSendClicked);
}

bool TelegramSenderWidget::eventFilter(QObject* obj, QEvent* event)
{
    if(obj == textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Return)
        {
            if(keyEvent->modifiers() & Qt::ShiftModifier)
            {
                textEdit->insertPlainText("\n");
            }
            else
            {
                onSendClicked();
            }
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void TelegramSenderWidget::onSendClicked()
{
    QString text = textEdit->toPlainText().trimmed();
    if(text.isEmpty()) return;
    sendMessage(text);
    textEdit->clear();
}

void TelegramSenderWidget::sendMessage(const QString &text)
{
    QUrl url(QString("https://api.telegram.org/bot%1/sendMessage").arg(botToken));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["chat_id"] = chatId;
    json["text"] = text;
    json["parse_mode"] = "HTML";

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    manager->post(request, data);
}


void TelegramSenderWidget::savePos(){
    QSettings s("tgpos.ini", QSettings::IniFormat);
    s.setValue("x", geometry().x());
    s.setValue("y", geometry().y());
    s.setValue("w", geometry().width());
    s.setValue("h", geometry().height());
    s.sync();
}

void TelegramSenderWidget::loadPos(){
    QSettings s("tgpos.ini", QSettings::IniFormat);
    setGeometry(s.value("x", 100).toInt(),
                s.value("y", 100).toInt(),
                s.value("w", 100).toInt(),
                s.value("h", 100).toInt()
                );

}
TelegramSenderWidget::~TelegramSenderWidget(){
    savePos();
}
