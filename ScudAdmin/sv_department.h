#ifndef SVDEPARTMENT_H
#define SVDEPARTMENT_H

#include <QObject>
#include <QDialog>
#include <QString>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextEdit>
#include <QVBoxLayout>

#include "ui_devicebrand.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"
#include "../../Common/log.h"
//#include "sv_select_"
#include "sv_employee.h"

#include "../defs_misscud.h"

class SvDepartment : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
    
    explicit SvDepartment(QWidget *parent, int departmentId = -1);
    ~SvDepartment();
    
    int showMode;
    
    int     t_id = -1;
    QString t_department_name = "";
    int     t_boss_id = -1;
    QString t_boss_name = "";
    QString t_boss_position = "";
    QString t_description = "";
    QByteArray   t_icon = QByteArray();

  private slots:
    void newBoss();
    void selectBoss();
    void loadIcon(QString fileName = "");

    void accept() Q_DECL_OVERRIDE;
    
  private:
    void setupUi();
        
    QVBoxLayout *vlayMain;
    QGroupBox *groupBox;
    QVBoxLayout *vlayDepartment;
    QHBoxLayout *hlayID;
    QLabel *lblID;
    QLineEdit *editID;
    QHBoxLayout *hlayDepartmentName;
    QLabel *lblDepartmentName;
    QLineEdit *editDepartmentName;
    QLabel *lblIcon;
    QPushButton *bnSelectIcon;
    QHBoxLayout *hlayBoss;
    QLabel *lblBossName;
    QLineEdit *editBossName;
    QPushButton *bnSelectBoss;
    QPushButton *bnNewBoss;
    QTextEdit *textDescription;
    QHBoxLayout *hlayButtons;
    QSpacerItem *spacer1;
    QPushButton *bnSave;
    QPushButton *bnCancel;
    
};

#endif // SVDEPARTMENT_H
