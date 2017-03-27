#ifndef SV_REPORTSETTINGSDIALOG_H
#define SV_REPORTSETTINGSDIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QSpinBox>

//#include "ui_report_settings.h"
#include "../../Common/log.h"
#include "../../Common/sv_pgdb.h"

#include "../defs_misscud.h"

//namespace Ui {
//  class SvReportSettingsDialog;
//}

class SvReportSettingsDialog : public QDialog
{
    Q_OBJECT
    
  public:
    explicit SvReportSettingsDialog(QWidget *parent = 0,
                                    bool showReportType = true,
                                    bool showEmployees = true,
                                    bool showPeriod = true,
                                    bool showMonthYear = false,
                                    int reportType = -1);
    ~SvReportSettingsDialog();
    
    int t_employee_id = -1;
    QString t_employee_name = "";
    
    int t_department_id = -1;
    QString t_department_name = "";
    
    QDate t_period_begin = QDate();
    QDate t_period_end = QDate();
    
    int t_month;
    int t_year;
    
    int t_report_type = 1;
    
  private slots:
    void employeeChange();
    void periodChange();
    
    void accept() Q_DECL_OVERRIDE;
    
  private:
//    Ui::SvReportSettingsDialog *ui;
    void setupUi(bool showReportType,
                 bool showEmployees,
                 bool showPeriod,
                 bool showMonthYear);
    
    void loadReportTypes();
    void loadDepartments();
    void loadEmployees();
    
    QVBoxLayout *verticalLayout;
    QGroupBox *gbReportTypes;
    QHBoxLayout *hlayReportTypes;
    QComboBox *cbReportTypes;
    QGroupBox *gbEmployee;
    QVBoxLayout *vlayEmployee;
    QRadioButton *rbAll;
    QHBoxLayout *hlayDepartment;
    QRadioButton *rbDepartment;
    QComboBox *cbDepartment;
    QHBoxLayout *hlayEmployee;
    QRadioButton *rbEmployee;
    QComboBox *cbEmployee;
    QGroupBox *gbPeriod;
    QVBoxLayout *vlayPeriod;
    QRadioButton *rbCurrentMonth;
    QRadioButton *rbLastMonth;
    QRadioButton *rbPeriod;
    QHBoxLayout *hlayPeriod;
    QLabel *lblPeriodBegin;
    QDateEdit *datePeriodBegin;
    QLabel *lblPeriodEnd;
    QDateEdit *datePeriodEnd;
    QGroupBox *gbMonthYear;
    QHBoxLayout *hlayMonthYear;
    QLabel *lblMonth;
    QComboBox *cbMonth;
    QLabel *lblYear;
    QSpinBox *spinYear;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *spacer1;
    QPushButton *bnOk;
    QPushButton *bnCancel;
};

#endif // SV_REPORTSETTINGSDIALOG_H
