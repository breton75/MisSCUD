#ifndef SV_DEVMODEL_H
#define SV_DEVMODEL_H

#include <QDialog>
#include <QString>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>

#include "sv_dev_brand.h"
#include "sv_dev_class.h"
#include "sv_select_brand_dialog.h"
#include "sv_select_class_dialog.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

#include "../defs_misscud.h"

namespace Ui {
  class SvDeviceModelDialog;
}

class SvDevModel : public QDialog
{
    Q_OBJECT
    
  private:

    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
    enum DataModel { dtMessage = 0, dtStream = 1 }; 
                    
    explicit SvDevModel(QWidget *parent,
                           int modelId = -1,
                           int showMode = smNew);
    
    ~SvDevModel();
    
    int showMode;
    
    int     t_id = -1;
    QString t_model_name = "";
    QString t_brand_name = "";
    QString t_class_name = "";
    int t_class_id = -1;
    int t_brand_id = -1;
    QString t_driver_path = "";
    QString t_description = "";
    int     t_dataType = dtMessage;
    
  public slots:
    void accept() Q_DECL_OVERRIDE;
    
    void slotNewClass();
    void slotSelectClass();
    
    void slotNewBrand();
    void slotSelectBrand();
    
    void slotSelectLibPath();
    
  private:
    Ui::SvDeviceModelDialog *dmDialog;
};

#endif // SV_DEVMODEL_H
