#ifndef SV_DEVICETYPE_H
#define SV_DEVICETYPE_H

#include <QDialog>
#include <QString>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

#include "ui_device.h"
#include "sv_dev_model.h"
#include "sv_select_model_dialog.h"
#include "sv_select_brand_dialog.h"
#include "sv_select_class_dialog.h"
#include "sv_device_config.h"

#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"

#include "../defs_misscud.h"

//#include "sv_zone_list.h"


namespace Ui {
  class SvDeviceDialog;
}

class SvDevice : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvDevice(QWidget *parent,
                      int deviceId = -1);
 
    
    ~SvDevice();
    
    int showMode;
    
    int     t_id = -1;
    QString t_device_name = "";
    qint32 t_ip = 0;
    int t_port = 0;
    int t_model_id = -1; // модель может быть не выбрана
    int t_class_id = -1;
    int t_brand_id = -1;
    QString t_model_name = "";
    QString t_class_name = "";
    QString t_brand_name = "";
    QString t_driver_path = "";
//    QString t_videoPath = "";
//    QString t_zoneName = "";
//    float   t_X = 0;
//    float   t_Y = 0;
//    int     t_Z;
    QString t_description = "";
    QByteArray t_config = QByteArray();
    
     
  private slots:
    void config();
    
  public slots:
    void accept() Q_DECL_OVERRIDE;
    void slotNewModel();
    void slotSelectModel();
    
  private:
    Ui::SvDeviceDialog *Dialog;

};

#endif // SV_DEVICETYPE_H
