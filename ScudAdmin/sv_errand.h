#ifndef SV_ERRAND_H
#define SV_ERRAND_H

#include <QDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"

#include "../defs_misscud.h"

#include "ui_sv_errand.h"

namespace Ui {
  class SvErrandDialog;
}

class SvErrand : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit };
    
    explicit SvErrand(QWidget *parent = 0, int errandId = -1);
    ~SvErrand();
    
    int t_id = -1;
    int t_employee_id = -1;
    QDateTime t_errand_begin = QDateTime::currentDateTime();
    QDateTime t_errand_end = QDateTime::currentDateTime();
    QString t_destination = "";
    QString t_description = "";
    
  private:
    Ui::SvErrandDialog *ui;
    
    int showMode;
    
    void accept() Q_DECL_OVERRIDE;
    void loadEmployees();
};

#endif // SV_ERRAND_H
