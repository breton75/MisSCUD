#ifndef SV_HOLIDAY_H
#define SV_HOLIDAY_H

#include <QDialog>
#include <QString>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>

#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

#include "ui_sv_holiday.h"

#include "../defs_misscud.h"

namespace Ui {
  class SvHolidayDialog;
}

class SvHoliday : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvHoliday(QWidget *parent,
                           int holidayId = -1,
                           int showMode = smNew);
    
    ~SvHoliday();
    
    int     t_id = -1;
    QString t_holiday_name = "";
    
    int showMode;
   
  private slots:
    void accept();
    
  signals:
    void sigDeviceAdded(int id);
    void sigDeviceDeleted(int id);
    
  private:
    Ui::SvHolidayDialog *Dialog;
};

#endif // SV_HOLIDAY_H
