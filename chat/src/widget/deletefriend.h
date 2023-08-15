#ifndef DELETEFRIEND_H
#define DELETEFRIEND_H

#include <QWidget>

namespace Ui {
class DeleteFriend;
}

class DeleteFriend : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteFriend(QString _userId, QString _friendId, QWidget *parent = nullptr);
    ~DeleteFriend();

signals:
    void deleteFriendSuccess();

private slots:
    void on_trueButton_clicked();

    void on_falseButton_clicked();

private:
    Ui::DeleteFriend *ui;
    QString userId;
    QString friendId;
};

#endif // DELETEFRIEND_H
