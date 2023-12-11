#include "userloginpage.h"
#include "./ui_userloginpage.h"

#include <QSqlQuery>
#include <QMessageBox>

userLoginPage::userLoginPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::userLoginPage)
{
    ui->setupUi(this);
    inintForm();
}

userLoginPage::~userLoginPage()
{
    delete ui;
}

void userLoginPage::inintForm()
{
    QString windTitle = "用户信息管理";
    this->setWindowTitle(windTitle);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//均匀拉伸表头
    QObject::connect(ui->pushButton_userEnroll,SIGNAL(clicked()),this,SLOT(userEnroll()));
    QObject::connect(ui->tableView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(tableViewEdit(const QModelIndex &)));
    QObject::connect(ui->pushButton_userInfoSave,SIGNAL(clicked()),this,SLOT(userInfoSave()));
    QObject::connect(ui->pushButton_userInfoDel,SIGNAL(clicked()),this,SLOT(userInfoDel()));

    initDB();
}

void userLoginPage::initDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");//数据库对象初始化，使用QSqlDatabase静态方法
    db.setHostName("jwh");
    db.setDatabaseName("jwhTEST.db");
    db.setUserName("admin");
    db.setPassword("J0a1m8");
    bool ok = db.open();

    if (!ok) {
        QMessageBox::about(this,"消息","Failed to connect to database");
        return ;
    }

    query = new QSqlQuery();
    query->exec("create table userinfo(useraccount,username,password)");//原来存在表，打开

    qmodel = new QSqlQueryModel();//数据模型
    updateTableView();
    ui->tableView->setModel(qmodel);//媒介
    qmodel->setHeaderData(0,Qt::Horizontal,"账号");
    qmodel->setHeaderData(1,Qt::Horizontal,"姓名");
    qmodel->setHeaderData(2,Qt::Horizontal,"密码");
}

void userLoginPage::updateTableView()
{
    qmodel->setQuery("select * from userinfo");
}

void userLoginPage::userEnroll()
{
    QString usrAcnt = ui->lineEdit_userAccount->text();
    QString usrName = ui->lineEdit_userName->text();
    QString usrPswd = ui->lineEdit_userPasswrd->text();
    query->prepare("SELECT * from userinfo WHERE useraccount = :useraccount");
    query->bindValue(":useraccount", usrAcnt);
    query->exec();
    // 检查结果集是否为空
    if (query->next()) {
        //QMessageBox::about(this,"消息","Username already exists");
        QMessageBox::about(this,"消息","用户存在");
        return;
    } else {
        QString sql = "insert into userinfo(useraccount,username,password) values('"+usrAcnt+"','"+usrName+"','"+usrPswd+"')";
        query->exec(sql);
        qmodel->setQuery("select * from userinfo");
        QMessageBox::about(this,"消息","注册成功");
    }

}

void userLoginPage::tableViewEdit(const QModelIndex &index)
{
    QString userAcnt = index.siblingAtColumn(0).data().toString();//模型.数据.转换int
    QString userName = index.siblingAtColumn(1).data().toString();
    QString userPswd = index.siblingAtColumn(2).data().toString();
    ui->lineEdit_userAccount->setText(userAcnt);
    ui->lineEdit_userName->setText(userName);
    ui->lineEdit_userPasswrd->setText(userPswd);
    curIndexModdel = index;
}

void userLoginPage::userInfoSave()
{
    QString usrAcnt = ui->lineEdit_userAccount->text();
    QString usrName = ui->lineEdit_userName->text();
    QString usrPswd = ui->lineEdit_userPasswrd->text();
    query->prepare("SELECT * from userinfo WHERE useraccount = :useraccount");
    query->bindValue(":useraccount", usrAcnt);
    query->exec();
    // 检查结果集是否为空
    if (query->next()) {
        QString sql = "update userinfo set username='"+usrName+"',password='"+usrPswd+"' where useraccount='"+usrAcnt+"'";
        query->exec(sql);
        updateTableView();

    } else {
        //QMessageBox::about(this,"消息","Useraccount not exists\n please enroll first");
        QMessageBox::about(this,"消息","用户不存在\n请先注册");
        return;
    }

}

void userLoginPage::userInfoDel()
{
    QString userAcnt = ui->lineEdit_userAccount->text();
    query->prepare("SELECT * from userinfo WHERE useraccount = :useraccount");
    query->bindValue(":useraccount", userAcnt);
    query->exec();
    // 检查结果集是否为空
    if (query->next()) {
        // QString userAcnt = curIndexModdel.siblingAtRow(0).data().toString();//有缺陷，删除所选项，从头继续删除
        // QString usrAcnt = ui->lineEdit_userAccount->text();//法1
        // ui->tableView->currentIndex();//法2

        QString sql = "delete from userinfo where useraccount='"+userAcnt+"'";
        query->exec(sql);
        updateTableView();

    } else {
        //QMessageBox::about(this,"消息","Useraccount not exists\n please enroll first");
        QMessageBox::about(this,"消息","用户不存在\n请先注册");
        return;
    }

}

