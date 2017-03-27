#ifndef SV_EMPLOYEE_H
#define SV_EMPLOYEE_H

#include <QDialog>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>

#include "../../Common/sv_pgdb.h"
#include "sv_scud.h"
#include "sv_select_scudkey_dialog.h"
#include "ui_sv_employee.h"
#include "sv_select_department_dialog.h"
#include "sv_department.h"

#include "../defs_misscud.h"

namespace Ui {
  class SvEmploeeDialog;
}

class SvEmployee : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
    
    explicit SvEmployee(QWidget *parent,
                        int emploeeID = -1);
    ~SvEmployee();
    
   int showMode; 
       int t_id = -1; 
   QString t_employee_name = "";
   QString t_employee_patronymic = "";
   QString t_employee_surename = "";
   QString t_short_name = "";
   QDate   t_birth_date;
   QString t_employee_position = "";
       int t_department_id = -1;
   QString t_department_name = "";
   QString t_email = "";
   QString t_phone1 = "";
   QString t_phone2 = "";
   QString t_description = "";
   QByteArray t_foto = QByteArray();

  private:
    Ui::SvEmploeeDialog *ui;
    
    QStandardItemModel* scudmodel;
    
  private slots:
    void loadFoto();
    void newDepartment();
    void selectDepartment();
    
    void newScudKey();
    void selectScudKey();
    void deleteScudKey();

    void accept() Q_DECL_OVERRIDE;
};

#endif // SV_EMPLOYEE_H
