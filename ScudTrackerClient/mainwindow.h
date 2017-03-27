#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QThread>

#include "ui_mainwindow.h"
#include "../../Common/log.h"
#include "../../Common/sql_select_defs.h"
#include "../../Common/sv_secondmeter.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sql_select_defs.h"
#include "../../Common/sv_tcpserverclient.h"
#include "sv_trackersettings.h"

#include "../defs_misscud.h"

/** эта структура соответствует структуре, 
 *  используемой в прошивке контроллера **/
struct GotKeyStruct
{
  uchar scud_number;
  uchar key_byte1;
  uchar key_byte2;
  uchar key_byte3;
  uchar sensor_command;
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
    
    SvTcpServer* tcp_server = 0;
//    QSqlQuery* query;
    
  public slots:
    void drawStatus(bool online);
    void drawLastData(QString lastData);
    
  private:
    Ui::MainWindow *ui;
    
    int t_key;
    int t_scud_number;
    int t_code;
    QDateTime t_date_time;
    bool debug = false;
    bool lastStatus = false;
    
    int last_key = -1;
    QDateTime last_date_time = QDateTime();
    
    QThread *workerThread;
    bool imgCounter = false;
    
    QString ip;
    quint16 port;
    
  private slots:
    void slotStart();
    void slotStop();
    void slotClose();
    void slotSettings();
    
    void on_bnTest_clicked();
    
  signals:
    void sigDBUpdated();
};

class ConnectToRemoteHost : public QObject
{
    Q_OBJECT
public:
    explicit ConnectToRemoteHost(QObject *parent, QString ip, quint16 port, int timeOut);
    ~ConnectToRemoteHost();

signals:

public slots:
    void getHostStatus();
    void getKeys();
    void disconnectFormHost();

private:
    QString ip;
    quint16 port;
    QTcpSocket *socket;
//    SvTcpClient * client;
    QTimer *timer;
    QTimer *awaitResponse;
    bool online = false;
    
    int last_key = -1;
    QDateTime last_date_time = QDateTime();
    
  signals:
    void status(bool);
    void lastData(QString);
    
};

#endif // MAINWINDOW_H
