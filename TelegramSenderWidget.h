// TelegramSenderWidget.h
#ifndef TELEGRAMSENDERWIDGET_H
#define TELEGRAMSENDERWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QNetworkAccessManager>

class TelegramSenderWidget : public QWidget
{
    Q_OBJECT
public:
    TelegramSenderWidget(const QString &token, const QString &chatId, QWidget *parent = nullptr);
    ~TelegramSenderWidget();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QTextEdit* textEdit;
    QPushButton* sendButton;
    QString botToken;
    QString chatId;
    QNetworkAccessManager* manager;

    void sendMessage(const QString &text);
    void savePos();
    void loadPos();

private slots:
    void onSendClicked();
};

#endif // TELEGRAMSENDERWIDGET_H
