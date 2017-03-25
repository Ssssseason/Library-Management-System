#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <QSqlRecord>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    ui->setupUi(this);
    ui->pushButton_quit->close();
    book = new QSqlTableModel(this);
    book->setTable("book");
    book->setEditStrategy(QSqlTableModel::OnManualSubmit);
    borrow = new QSqlTableModel(this);
    borrow->setTable("borrow");
    borrow->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    QSqlQueryModel *writecard = new QSqlQueryModel;
    ui->lineEdit_PW->setEchoMode(QLineEdit::Password);
    ui->tabWidget_2->show();
    ui->tabWidget->close();
    ui->lineEdit_return_bno->hide();
    ui->label_return_bno->hide();
    ui->lineEdit_borrow_bno->hide();
    ui->label_borrow_bno->hide();

        ui->comboBox_type->addItem("");
        ui->comboBox_type->addItem(QStringLiteral("书名"));
        ui->comboBox_type->addItem(QStringLiteral("年份"));
        ui->comboBox_type->addItem(QStringLiteral("价格"));
        ui->comboBox_type->addItem(QStringLiteral("库存"));
        ui->comboBox_adde->addItem("");
        ui->comboBox_adde->addItem(QStringLiteral("升序"));
        ui->comboBox_adde->addItem(QStringLiteral("降序"));

        ui->comboBox_type_2->addItem("");
        ui->comboBox_type_2->addItem(QStringLiteral("书名"));
        ui->comboBox_type_2->addItem(QStringLiteral("年份"));
        ui->comboBox_type_2->addItem(QStringLiteral("价格"));
        ui->comboBox_type_2->addItem(QStringLiteral("库存"));
        ui->comboBox_adde_2->addItem("");
        ui->comboBox_adde_2->addItem(QStringLiteral("升序"));
        ui->comboBox_adde_2->addItem(QStringLiteral("降序"));
        return_flag = 0;
        borrow_flag = 0;
//        user = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::project_borrowedbook(QString cno){
    QSqlQuery query;
    QString judge;
    query.exec(QString("select bno from borrow where cno = '%1' and isnull(return_date)").arg(cno));
    query.next();
    int num = query.size();
    for(int i = 0; i < num; i++){
        QString bno = query.value(0).toString();
        query.next();
        if(i == 0)
            judge = QString("bno = '%1'").arg(bno);
        else judge += QString(" or bno = '%1'").arg(bno);
    }
            qDebug() << judge << num;
    book->setFilter(judge);
    book->select();
    if(num){
        ui->tableView->setModel(book);
    }
    else ui->tableView->setModel(NULL);

    return num;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 2 || index == 3 || index == 4)
        ui->tableView->setModel(NULL);
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
        ui->tabWidget_2->show();
        ui->tabWidget->close();
        ui->lineEdit_UN->clear();
        ui->lineEdit_PW->clear();
//        user = 0;
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
        ui->lineEdit_UN->clear();
        ui->lineEdit_PW->clear();
    }else {
        QMessageBox::warning(this,QStringLiteral("管理员登录"),QStringLiteral("登录成功"),QMessageBox::Ok);
        ui->label_role->setText(UN);
        ui->lineEdit_PW->hide();
        ui->lineEdit_UN->hide();
        ui->pushButton_login->hide();
        ui->pushButton_quit->show();
        ui->tabWidget_2->close();
        ui->tabWidget->show();
//        user = 1;
        USER = UN;
    }
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
    int type = ui->comboBox_type->currentIndex();
    if(type == 0 || type == 1)
        type = 2;
    else if(type == 2) type = 4;
    else if(type == 3) type = 6;
    else type = 8;
    int adde = ui->comboBox_adde->currentIndex();

    int flag = 0;
    QString judge, sort;
    if(!QString(category).isEmpty()){
        judge += QString("category = '%1'").arg(category);
        flag++;
    }
    if(!QString(title).isEmpty()){
        if(flag)
            judge += " and ";
        judge += QString("title = '%1'").arg(title);
        flag++;
    }
    if(!QString(press).isEmpty()){
        if(flag)
            judge += " and ";
        judge += QString("press = '%1'").arg(press);
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
        judge += QString("author = '%1'").arg(author);
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
        judge += "price <= " + price2;
        flag++;
    }

    qDebug() << flag;
    qDebug() << judge;
    book->setFilter(judge);
    book->setSort(type, Qt::AscendingOrder);
    if(adde == 2)
        book->setSort(type, Qt::DescendingOrder);
    book->select();
    ui->tableView->setModel(book);
}

void MainWindow::on_pushButton_book_select_2_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QString category = ui->lineEdit_category_2->text();
    QString title = ui->lineEdit_title_2->text();
    QString press = ui->lineEdit_press_2->text();
    QString year1 = ui->lineEdit_year1_2->text();
    QString year2 = ui->lineEdit_year2_2->text();
    QString author = ui->lineEdit_author_2->text();
    QString price1 = ui->lineEdit_price1_2->text();
    QString price2 = ui->lineEdit_price2_2->text();
    int type = ui->comboBox_type_2->currentIndex();
    if(type == 0 || type == 1)
        type = 2;
    else if(type == 2) type = 4;
    else if(type == 3) type = 6;
    else type = 8;
    int adde = ui->comboBox_adde_2->currentIndex();

    int flag = 0;
    QString judge, sort;
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
        judge += "price <= " + price2;
        flag++;
    }

//    qDebug() << flag;
//    qDebug() << judge;
    book->setFilter(judge);
    book->setSort(type, Qt::AscendingOrder);
    if(adde == 2)
        book->setSort(type, Qt::DescendingOrder);
    book->select();
    ui->tableView->setModel(book);
}

void MainWindow::on_pushButton_deletecard_clicked()
{
    QString cno = ui->lineEdit_card_cno->text();
    QSqlQuery query;
    query.prepare("delete from card where cno = ?");
    query.addBindValue(cno);
    if(!query.exec()) {
        QMessageBox::warning(this,QStringLiteral("删除借书证"),QStringLiteral("有未还书籍"),QMessageBox::Ok);
        project_borrowedbook(cno);
    }else {
        QMessageBox::warning(this,QStringLiteral("删除借书证"),QStringLiteral("删除成功"),QMessageBox::Ok);
        ui->lineEdit_card_cno->clear();
        ui->tableView->setModel(NULL);
    }
}

void MainWindow::on_pushButton_addcard_clicked()
{
    QString cno = ui->lineEdit_card_cno->text();
    QSqlQuery query;
    query.prepare("insert into card(cno) values (:cno)");
    query.addBindValue(cno);
    if(!query.exec()) {
        QMessageBox::warning(this,QStringLiteral("添加借书证"),QStringLiteral("添加失败"),QMessageBox::Ok);
    }else {
        QMessageBox::warning(this,QStringLiteral("删除借书证"),QStringLiteral("添加成功"),QMessageBox::Ok);
        ui->lineEdit_card_cno->clear();
    }
}

void MainWindow::on_pushButton_return_clicked()
{
    QSqlQuery query;
    QString judge;
    if(return_flag == 0){
        // project all info of book borrowed
        return_cno = ui->lineEdit_return_cno->text();
        int num = project_borrowedbook(return_cno);
        query.exec(QString("select * from card where cno = '%1'").arg(return_cno));
        query.next();
        int card_exist = query.size();
        if(!card_exist){
            QMessageBox::warning(this,QStringLiteral("还书"),QStringLiteral("借书证不存在"),QMessageBox::Ok);
            ui->lineEdit_return_cno->clear();
        }else if(!num){
            // no book borrowed
            QMessageBox::warning(this,QStringLiteral("还书"),QStringLiteral("无待还书籍"),QMessageBox::Ok);
            ui->tableView->setModel(NULL);
            ui->lineEdit_return_cno->clear();
        }
        else if(!return_cno.isEmpty() && query.size()){
            ui->lineEdit_return_bno->show();
            ui->label_return_bno->show();
            ui->lineEdit_return_cno->hide();
            ui->label_return_cno->hide();
            ui->tableView->setModel(book);
            return_flag = 1;
        }
    }else {
        int exist = 0, i;
        QSqlRecord record_book;
        QString return_bno = ui->lineEdit_return_bno->text();
        // get location of book required to be return
        for(i = 0; i < book->rowCount(); i++){
            record_book = book->record(i);
            if(return_bno == record_book.value(0).toString()){
                exist = 1;
                break;
            }
        }
        query.exec(QString("select * from borrow where cno = '%1' and bno = '%2' and isnull(return_date)").arg(return_cno, return_bno));
        query.next();
        int size = query.size();
        // book has't been returned
        if(exist && size){
            // stock += 1
            record_book.setValue("stock", record_book.value("stock").toInt()+1);
            book->setRecord(i, record_book);
            QDateTime time = QDateTime::currentDateTime();
            QString return_time = time.toString("yyyy-MM-dd hh:mm:ss");
            query.exec(QString("update borrow set return_date = '%1', ID = '%2' where cno = '%3' and bno = '%4' and isnull(return_date)").arg(return_time, USER, return_cno, return_bno));
            if(book->submitAll())
                QMessageBox::warning(this,QStringLiteral("还书"),QStringLiteral("还书成功"),QMessageBox::Ok);
            else QMessageBox::warning(this,QStringLiteral("还书"),QStringLiteral("还书失败"),QMessageBox::Ok);
            project_borrowedbook(return_cno);
        }
        else {
            QMessageBox::warning(this,QStringLiteral("还书"),QStringLiteral("未借该书"),QMessageBox::Ok);
        }
        ui->lineEdit_return_bno->hide();
        ui->label_return_bno->hide();
        ui->lineEdit_return_cno->show();
        ui->label_return_cno->show();
        ui->lineEdit_return_bno->clear();
        return_flag = 0;
    }
}

void MainWindow::on_pushButton_borrow_clicked()
{
    QSqlQuery query;
    QString judge;
    if(borrow_flag == 0){
        // project all info of book borrowed
        borrow_cno = ui->lineEdit_borrow_cno->text();
        int num = project_borrowedbook(borrow_cno);
        query.exec(QString("select * from card where cno = '%1'").arg(borrow_cno));
        query.next();
        int card_exist = query.size();
        if(card_exist && num < 2){
            ui->lineEdit_borrow_bno->show();
            ui->label_borrow_bno->show();
            ui->lineEdit_borrow_cno->hide();
            ui->label_borrow_cno->hide();
            borrow_flag = 1;
        }else if(!card_exist){
            // invalid card
            QMessageBox::warning(this,QStringLiteral("借书"),QStringLiteral("借书证不存在"),QMessageBox::Ok);
            ui->lineEdit_borrow_cno->clear();
        }else {
            QMessageBox::warning(this,QStringLiteral("借书"),QStringLiteral("请先还书"),QMessageBox::Ok);
            ui->lineEdit_borrow_cno->clear();
        }
    }else {
        int stock = 0, i;
        QSqlRecord record_book;
        QString borrow_bno = ui->lineEdit_borrow_bno->text();
        // get stock of book required to be borrowed
        query.exec(QString("select stock from book where bno = '%1'").arg(borrow_cno));
        stock = query.value(0).toInt();
        book->setFilter("");
        book->select();
        for(i = 0; i < book->rowCount(); i++){
            record_book = book->record(i);
            qDebug() << "i = " << i;
            if(borrow_bno == record_book.value(0).toString() && record_book.value(8).toInt()){
                stock = record_book.value(8).toInt();
                break;
            }
        }
//        qDebug() << stock;
//        qDebug() << record_book;
        if(stock){
            // stock -= 1
            record_book.setValue("stock", record_book.value("stock").toInt()-1);
            book->setRecord(i, record_book);
            QDateTime time = QDateTime::currentDateTime();
            QString borrow_time = time.toString("yyyy-MM-dd hh:mm:ss");
            query.exec(QString("insert into borrow (cno, bno, borrow_date, ID)"
                               "values ('%1', '%2', '%3', '%4')").arg(borrow_cno, borrow_bno, borrow_time, USER));
            if(book->submitAll()){
                project_borrowedbook(borrow_cno);
                QMessageBox::warning(this,QStringLiteral("借书"),QStringLiteral("借书成功"),QMessageBox::Ok);
            }
            else QMessageBox::warning(this,QStringLiteral("借书"),QStringLiteral("借书失败"),QMessageBox::Ok);
        }else{
            QMessageBox::warning(this,QStringLiteral("借书"),QStringLiteral("库存为零"),QMessageBox::Ok);
        }

        ui->lineEdit_borrow_bno->hide();
        ui->label_borrow_bno->hide();
        ui->lineEdit_borrow_cno->show();
        ui->label_borrow_cno->show();
        ui->lineEdit_borrow_bno->clear();
        borrow_flag = 0;
    }
}

void MainWindow::on_pushButton_openfile_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString fileName = QFileDialog :: getOpenFileName(this, NULL, NULL, "*.txt");
    QFile file(fileName);
    if (file.open(QIODevice ::ReadOnly | QIODevice ::Text))
    {
        ui->textEdit->clear();
        QTextStream textStream(&file);
        textStream.setCodec("GBK");
        while (!textStream.atEnd())
            ui->textEdit->append(textStream.readLine());
    }
    else
        QMessageBox::warning(this,QStringLiteral("打开文件"),QStringLiteral("失败"),QMessageBox::Ok);
}

void MainWindow::on_pushButton_insert_clicked()
{
    QString value = ui->textEdit->toPlainText(), select;
    QTextStream valuestream(&value);
    QString bno, category, title, press, author, year, price, record;
    int number, total, stock, i = 0;
    QSqlQuery query;
    while(!valuestream.atEnd()){
        i++;
        record = valuestream.readLine();
        bno = record.section(',',0,0);
        category = record.section(',',1,1);
        title = record.section(',',2,2);
        press = record.section(',',3,3);
        year = record.section(',',4,4);
        author = record.section(',',5,5);
        price = record.section(',',6,6);
        number = record.section(',',7,7).toInt();

        query.exec(QString("select * from book where bno = '%1'").arg(bno));
        query.next();
        if(query.size()){
            total = query.value(7).toInt() + number;
            stock = query.value(8).toInt() + number;
            select = QString("update book set total = '%1',stock = '%2' where bno = '%3'").arg(QString::number(total), QString::number(stock), bno);
            if(query.exec(QString("update book set total = '%1',stock = '%2' where bno = '%3'").arg(QString::number(total), QString::number(stock), bno)))
                QMessageBox::warning(this,QStringLiteral("图书入库"),QStringLiteral("成功"),QMessageBox::Ok);
        }else {
            total = stock = number;
            select = QString("insert into book values ('%1', '%2', '%3', '%4', '%5','%6', '%7', '%8', '%9')").arg(bno, category, title, press, year, author, price, QString::number(total), QString::number(stock));
            if(query.exec(select))
                QMessageBox::warning(this,QStringLiteral("图书入库"),QString("第'i'条插入成功").arg(QString::number(i)),QMessageBox::Ok);
        }
        qDebug() << bno << category << title << press << year;
        qDebug() << author << price << number;
        qDebug() << select;
    }
}
