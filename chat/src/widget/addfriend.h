#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>

namespace Ui {
class AddFriend;
}

class AddFriend : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriend(QString _userId, QWidget *parent = nullptr);
    ~AddFriend();

signals:
    void addFriendSuccess();

private slots:
    void on_addButton_clicked();

private:
    Ui::AddFriend *ui;
    QString userId;
};

#endif // ADDFRIEND_H
