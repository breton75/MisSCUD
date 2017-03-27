#ifndef SV_DEVICECLASS_H
#define SV_DEVICECLASS_H

#include <QDialog>
#include <QString>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFileDialog>

#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

#include "ui_sv_deviceclass.h"

#include "../defs_misscud.h"

namespace Ui {
  class SvClassDialog;
}

class SvDeviceClass : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvDeviceClass(QWidget *parent,
                           int classId = -1,
                           int showMode = smNew);
    
    ~SvDeviceClass();
    
    int     t_id = -1;
    QString t_class_name = "";
    QString t_driver_path = "";
    QString t_description = "";
    QByteArray t_icon = QByteArray();
//    QPixmap t_icon = QPixmap();
    
    int showMode;
     
    
  private slots:
    void accept();
    void selectIcon();
    void slotSelectLibPath();
    
  signals:
    void sigDeviceAdded(int id);
    void sigDeviceDeleted(int id);
    
  private:
    Ui::SvClassDialog *Dialog;
};

#endif // SV_DEVICECLASS_H
