#ifndef SV_TRACKERSETTINGS_H
#define SV_TRACKERSETTINGS_H

#include <QDialog>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

#include "ui_sv_trackersettings.h"

#include "../../Common/sv_pgdb.h"
#include "../sv_select_device_dialog.h"

namespace Ui {
  class SvTrackerSettings;
}

class SvTrackerSettings : public QDialog
{
    Q_OBJECT
    
  public:
    explicit SvTrackerSettings(QWidget *parent = 0);
    ~SvTrackerSettings();
    
  private:
    Ui::SvTrackerSettings *ui;
    
  private slots:
    void accept() Q_DECL_OVERRIDE;
    
    void addDevice();
    void deleteDevice();
    
};

#endif // SV_TRACKERSETTINGS_H
