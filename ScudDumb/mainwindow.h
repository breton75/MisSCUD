#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QThread>
#include <QFile>
#include <QMessageBox>

#include "ui_mainwindow.h"
#include "../../Common/log.h"
#include "../../Common/sql_select_defs.h"
#include "../../Common/sv_secondmeter.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sql_select_defs.h"
#include "../../Common/sv_tcpserverclient.h"

#include "../defs_misscud.h"

/** эта структура соответствует структуре, 
 *  используемой в прошивке контроллера **/
struct GotKeyStruct
{
  uchar scud_number;
  uchar key_byte1;
  uchar key_byte2;
  uchar key_byte3;
//  uchar sec;
//  uchar min;
//  uchar hour;
//  uchar day;
//  uchar month;
//  uchar weekday;
//  uchar year;
};

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void AfterShow();
    
    QTextEdit* get_log();
    
  private:
    Ui::MainWindow *ui;
    
    int t_key;
    int t_scud_number;
    int t_code;
    QDateTime t_date_time;

    bool debug = false;
    bool lastStatus = false;
    
  private slots:
    void slotSave();
    void loadDepartments();
    void loadEmployees(int idx);
    
};


#endif // MAINWINDOW_H
