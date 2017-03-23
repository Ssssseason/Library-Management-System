#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_quit->close();
    book = new QSqlTableModel(this);
    book->setTable("book");
    book->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    for(int i = 1; i < 8; i++)
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(3);
        ui->tabWidget->removeTab(4);

    user = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_quit_clicked()
{
    if(createConnection("guest", "")){
        QMessageBox::warning(this,QStringLiteral("管理员登出"),QStringLiteral("登出成功"),QMessageBox::Ok);
        ui->label_role->setText("guest");
        ui->lineEdit_UN->show();
        ui->lineEdit_PW->show();
        ui->pushButton_login->show();
        ui->pushButton_quit->hide();
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(3);
        ui->tabWidget->removeTab(4);
        user = 0;
    }else QMessageBox::warning(this,QStringLiteral("管理员登出"),QStringLiteral("登出失败"),QMessageBox::Ok);
}

void MainWindow::on_pushButton_login_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString UN = ui->lineEdit_UN->text();
    QString PW = ui->lineEdit_PW->text();
    if(!createConnection(UN, PW)){
        QMessageBox::warning(this,QStringLiteral("管理员登录"),QStringLiteral("登录失败"),QMessageBox::Ok);
        createConnection("guest", "");
    }else {
        QMessageBox::warning(this,QStringLiteral("管理员登录"),QStringLiteral("登录成功"),QMessageBox::Ok);
        ui->label_role->setText(UN);
        ui->lineEdit_PW->hide();
        ui->lineEdit_UN->hide();
        ui->pushButton_login->hide();
        ui->pushButton_quit->show();
        ui->tabWidget->addTab(ui->tabWidget, QStringLiteral("图书入库"));
//        ui->tabWidget->addTab(2);
//        ui->tabWidget->addTab(3);
//        ui->tabWidget->addTab(4);
        user = 1;
    }
}

void MainWindow::show_book_select(){
    ui->label_author->show();
    ui->label_category->show();
    ui->label_press->show();
    ui->label_price->show();
    ui->label_title->show();
    ui->label_year->show();
    ui->lineEdit_author->show();
    ui->lineEdit_category->show();
    ui->lineEdit_press->show();
    ui->lineEdit_price1->show();
    ui->lineEdit_price2->show();
    ui->lineEdit_title->show();
    ui->lineEdit_year1->show();
    ui->lineEdit_year2->show();
    ui->label_between1->show();
    ui->label_between2->show();
}

void MainWindow::hide_book_select(){
    ui->label_author->hide();
    ui->label_category->hide();
    ui->label_press->hide();
    ui->label_price->hide();
    ui->label_title->hide();
    ui->label_year->hide();
    ui->lineEdit_author->hide();
    ui->lineEdit_category->hide();
    ui->lineEdit_press->hide();
    ui->lineEdit_price1->hide();
    ui->lineEdit_price2->hide();
    ui->lineEdit_title->hide();
    ui->lineEdit_year1->hide();
    ui->lineEdit_year2->hide();
    ui->label_between1->hide();
    ui->label_between2->hide();
}

void MainWindow::on_pushButton_book_select_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QString category = ui->lineEdit_category->text();
    QString title = ui->lineEdit_title->text();
    QString press = ui->lineEdit_press->text();
    QString year1 = ui->lineEdit_year1->text();
    QString year2 = ui->lineEdit_year2->text();
    QString author = ui->lineEdit_author->text();
    QString price1 = ui->lineEdit_price1->text();
    QString price2 = ui->lineEdit_price2->text();

    int flag = 0;
    QString judge;
    if(!QString(category).isEmpty()){
        judge += "category = " + category;
        flag++;
    }
    if(!QString(title).isEmpty()){
        if(flag)
            judge += " and ";
        judge += "title = " + title;
        flag++;
    }
    if(!QString(press).isEmpty()){
        if(flag)
            judge += " and ";
        judge += "press = " + press;
        flag++;
    }
    if(!QString(year1).isEmpty()){
        if(flag)
            judge += " and ";
        judge += "year >= " + year1;
        flag++;
    }
    if(!QString(year2).isEmpty()){
        if(flag)
            judge += " and ";
        judge += "year <= " + year2;
        flag++;
    }
    if(!QString(author).isEmpty()){
        if(flag)
            judge += " and ";
        judge += "author >= " + author;
        flag++;
    }
    if(!QString(price1).isEmpty()){
        if(flag)
            judge += " and ";
        judge += "price >= " + price1;
        flag++;
    }
    if(!QString(price2).isEmpty()){
        if(flag)
            judge += " and ";
        judge += "price >= " + price2;
        flag++;
    }
    qDebug() << flag;
    qDebug() << judge;
    book->setFilter(judge);
    book->setSort(0, Qt::AscendingOrder);
    book->select();
    ui->tableView->setModel(book);
}

