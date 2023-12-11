#ifndef USERLOGINPAGE_H
#define USERLOGINPAGE_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QSqlQueryModel>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class userLoginPage; }
QT_END_NAMESPACE

class userLoginPage : public QMainWindow
{
    Q_OBJECT

public:
    userLoginPage(QWidget *parent = nullptr);
    ~userLoginPage();

private:
    Ui::userLoginPage *ui;
    void inintForm();
    void initDB();
    void updateTableView();
    QSqlQuery *query;
    QSqlDatabase db;
    QSqlQueryModel *qmodel;//不同的方法中调用，全局变量
    QModelIndex curIndexModdel;
private slots:
    void userEnroll();
    void tableViewEdit(const QModelIndex  &index);
    void userInfoSave();
    void userInfoDel();
};
#endif // USERLOGINPAGE_H
