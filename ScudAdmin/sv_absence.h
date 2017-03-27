#ifndef SV_ABSENCE_H
#define SV_ABSENCE_H

#include <QDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"

#include "../defs_misscud.h"

namespace Ui {
  class SvAbsenceDialog;
}

class SvAbsence : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit };
    
    explicit SvAbsence(QWidget *parent = 0, int absenceId = -1);
    ~SvAbsence();
    
    int t_id = -1;
    int t_absence_id = -1;
    int t_employee_id = -1;
    QDate t_period_begin = QDate::currentDate();
    QDate t_period_end = QDate::currentDate();
    
  private:
    Ui::SvAbsenceDialog *ui;
    
    int showMode;
    
    void accept() Q_DECL_OVERRIDE;
    void loadEmployees();
    void loadAbsences();
    
};

#endif // SV_ABSENCE_H
