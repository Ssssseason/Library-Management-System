#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <Qtsql/QSqlDatabase>
#include <QSqlQuery>
#include <string>
#include <QDataStream>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QTextStream>
#include <QtSql/qsqldriver.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if(!createConnection("root", "5183685")) {
        qDebug() << "Error!";
        return 1;
    }
    qDebug() << "Successfully Connected!";

    MainWindow w;
    w.setWindowTitle(QStringLiteral("图书管理系统"));
    w.show();

    return a.exec();
}
