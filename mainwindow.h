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
    void on_tabWidget_tabBarClicked(int index);

    int project_borrowedbook(QString cno);

    void on_pushButton_quit_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_book_select_clicked();

    void on_pushButton_book_select_2_clicked();

    void on_pushButton_deletecard_clicked();

    void on_pushButton_addcard_clicked();

    void on_pushButton_return_clicked();

    void on_pushButton_borrow_clicked();

    void on_pushButton_openfile_clicked();

    void on_pushButton_insert_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *book;
    QSqlTableModel *borrow;
    QSqlQueryModel *writecard;
    int user;
    int return_flag;
    int borrow_flag;
    QString return_cno;
    QString borrow_cno;
    QString USER;
};

#endif // MAINWINDOW_H
