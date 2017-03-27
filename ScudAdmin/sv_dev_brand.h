#ifndef SV_DEVICEBRAND_H
#define SV_DEVICEBRAND_H

#include <QDialog>
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

#include "ui_devicebrand.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

#include "../defs_misscud.h"

namespace Ui {
  class SvBrandDialog;
}

class SvBrand : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvBrand(QWidget *parent,
                     int brandId = -1);
    
    ~SvBrand();
    
    int showMode;
    
    int     t_id = -1;
    QString t_brand_name = "";
    QString t_description = "";
    
  public slots:
    virtual void accept();
    
  private:
    Ui::SvBrandDialog *Dialog;
};

#endif // SV_DEVICEBRAND_H
