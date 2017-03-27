#ifndef SV_SCUD_H
#define SV_SCUD_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>

#include "../../Common/sv_tcpserverclient.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"
#include "ui_sv_scud.h"
//#include "./Common/sv_device_list.h"
//#include "sv_zone.h"
//#include "./Common/sv_zone_list.h"
#include "sv_employee.h"
//#include "./Common/sv_employees_list.h"

#include "../defs_misscud.h"

struct GotKeyStruct
{
  uchar scud_number;
  uchar key_byte1;
  uchar key_byte2;
  uchar key_byte3;
  uchar sec;
  uchar min;
  uchar hour;
  uchar day;
  uchar month;
  uchar weekday;
  uchar year;
};

namespace Ui {
  class SvSCUD;
}

class AwaitingKey;

class SvSCUD : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode
    { 
      smNew = 0,
      smEdit
    };
    
    explicit SvSCUD(QWidget *parent,
                    int scudKeyId = -1);
    
    ~SvSCUD();
    
    int t_id = -1;
    int t_key_value = 0;
    int t_device_id = -1;
    QString t_device_name = "";
    int t_employee_id = -1;
    QString t_employee_name = "";
    QString t_employee_patronymic = "";
    QString t_employee_surename = "";
    QString t_employee_position = "";
    QString t_short_name = "";
//    QString t_zone_name = "";
    
    int showMode;
    
  private:
    void accept() Q_DECL_OVERRIDE;
    
    Ui::SvSCUD *ui;
    
    SvTcpClient* tcp_client = 0;
    AwaitingKey* awaitingKey;
    
  private slots:
    void slotGetKey();
//    void on_AWAITING_closed();
    
    void on_bnSelectEmployee_clicked();
    void on_bnNewEmployee_clicked();
    
    void slotSelectDevice(); 
    
};

class AwaitingKey: public QDialog
{
    Q_OBJECT
    
  public:
    explicit AwaitingKey(QWidget *parent, QString ip = "");
    
    ~AwaitingKey();
    
    void accept() Q_DECL_OVERRIDE;
    void reject() Q_DECL_OVERRIDE;
    
    int t_scud_number;
    int t_code;
    int t_key;
    QDateTime t_date_time;
    int t_weekday;
    
  private:
    SvTcpServer* tcp_server = 0;
        
};

#endif // SV_SCUD_H
