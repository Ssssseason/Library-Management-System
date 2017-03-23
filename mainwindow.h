#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlquery.h>
#include <Qtsql/QSqlDatabase>
#include <QSqlTableModel>

static bool createConnection(QString UN, QString PW) {
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName("jyx3150102305");
  db.setUserName(UN);
  db.setPassword(PW);

  db.exec("set Names 'UTF8'");
  if (!db.open())
    return false;
  return true;
}


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void show_book_select();
    void hide_book_select();

    void on_pushButton_quit_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_book_select_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *book;
    int user;
};

#endif // MAINWINDOW_H
