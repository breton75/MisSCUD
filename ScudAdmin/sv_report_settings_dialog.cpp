#include "sv_report_settings_dialog.h"

using namespace log_ns;

SvReportSettingsDialog* REPORTSETTINGS_UI;
extern SvPGDB *PGDB;

SvReportSettingsDialog::SvReportSettingsDialog(QWidget *parent,
                                               bool showReportType,
                                               bool showEmployees,
                                               bool showPeriod,
                                               bool showMonthYear,
                                               int reportType) :
  QDialog(parent)
//  ui(new Ui::SvReportSettingsDialog)
{
  setupUi(showReportType, showEmployees, showPeriod, showMonthYear);  
  
  datePeriodBegin->setDate(QDate::currentDate());
  datePeriodEnd->setDate(QDate::currentDate());
  
  cbMonth->setCurrentIndex(QDate::currentDate().month() - 1);
  spinYear->setValue(QDate::currentDate().year());
  
  loadReportTypes();
  if(reportType != -1) cbReportTypes->setCurrentIndex(cbReportTypes->findData(reportType));
  
  loadDepartments();
  loadEmployees();
  
  connect(rbAll, SIGNAL(clicked()), this, SLOT(employeeChange()));
  connect(rbDepartment, SIGNAL(clicked()), this, SLOT(employeeChange()));
  connect(rbEmployee, SIGNAL(clicked()), this, SLOT(employeeChange()));
  
  connect(rbCurrentMonth, SIGNAL(clicked()), this, SLOT(periodChange()));
  connect(rbLastMonth, SIGNAL(clicked()), this, SLOT(periodChange()));
  connect(rbPeriod, SIGNAL(clicked()), this, SLOT(periodChange()));
  
  connect(bnOk, SIGNAL(clicked()), this, SLOT(accept()));
  connect(bnCancel, SIGNAL(clicked()), this, SLOT(reject()));
  
  this->setModal(true);
  this->show();
  
}

SvReportSettingsDialog::~SvReportSettingsDialog()
{
  close();
  deleteLater();
}

void SvReportSettingsDialog::loadReportTypes()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_REPORT_TYPES).arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    cbReportTypes->addItem(q->value("report_type_name").toString(),
                              q->value("report_type_id").toInt());
  }

  q->finish();
  
  if(cbReportTypes->count()) cbReportTypes->setCurrentIndex(0);
}

void SvReportSettingsDialog::loadDepartments()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_DEPARTMENTS_LIST).arg(""), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    QPixmap pix = QPixmap(16, 16);
    pix.loadFromData(q->value("icon").toByteArray());
    cbDepartment->addItem(QIcon(pix.scaled(16, 16, Qt::KeepAspectRatio)),
                         q->value("department_name").toString(),
                         q->value("department_id").toInt());
  }

  q->finish();
  
  if(cbDepartment->count()) cbDepartment->setCurrentIndex(0);
}

void SvReportSettingsDialog::loadEmployees()
{
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_EMPLOYEES_LIST_LIGHT).arg("where deleted = false"), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
  {
    cbEmployee->addItem(QString("%1 (%2)")
                            .arg(q->value("short_name").toString())
                            .arg(q->value("employee_position").toString()),
                         q->value("employee_id").toInt());
  }

  q->finish();
  
  if(cbEmployee->count()) cbEmployee->setCurrentIndex(0);
}

void SvReportSettingsDialog::employeeChange()
{
  foreach (QRadioButton *rb, gbEmployee->findChildren<QRadioButton*>())
    rb->setChecked(rb == (QRadioButton *)sender());
    
  cbDepartment->setEnabled(rbDepartment->isChecked());
  cbEmployee->setEnabled(rbEmployee->isChecked());
  
}

void SvReportSettingsDialog::periodChange()
{
  foreach (QRadioButton *rb, gbPeriod->findChildren<QRadioButton*>())
    rb->setChecked(rb == (QRadioButton *)sender());
  
  lblPeriodBegin->setEnabled(rbPeriod->isChecked());
  lblPeriodEnd->setEnabled(rbPeriod->isChecked());
  datePeriodBegin->setEnabled(rbPeriod->isChecked());
  datePeriodEnd->setEnabled(rbPeriod->isChecked());
}

void SvReportSettingsDialog::accept()
{
  /* тип отчета */
  t_report_type = cbReportTypes->currentData().toInt();
  
  /* отдел или сотрудник */
  if(rbDepartment->isChecked())
  {
    t_department_id = cbDepartment->currentData().toInt();
    t_department_name = cbDepartment->currentText();
  }
  
  else if(rbEmployee->isChecked())
  {
    t_employee_id = cbEmployee->currentData().toInt();
    t_employee_name = cbEmployee->currentText();
  }
  
  else
  {
    t_department_id = -1;
    t_employee_id = -1;
  }
  
  /* период */
  if(rbCurrentMonth->isChecked())
  {
    QDate d = QDate::currentDate();
    t_period_begin.setDate(d.year(), d.month(), 1);
    t_period_end.setDate(d.year(), d.month(), d.daysInMonth());
  }
  
  else if(rbLastMonth->isChecked()) 
  {
    QDate d = QDate::currentDate();
    int m = (d.month() == 1 ? 12 : d.month() - 1);
    int y = (d.month() == 1 ? d.year() - 1 : d.year());
    
    t_period_begin.setDate(y, m, 1);
    t_period_end.setDate(y, m, t_period_begin.daysInMonth());
  }
  
  else
  {
    if(!datePeriodBegin->date().isValid())
    {
      log_ns::log(m_Critical, "Неверно указана дата начала периода");
      datePeriodBegin->setFocus();
      return;
    }    
    
    if(!datePeriodEnd->date().isValid())
    {
      log_ns::log(m_Critical, "Неверно указана дата конца периода");
      datePeriodEnd->setFocus();
      return;
    }
    
    t_period_begin = datePeriodBegin->date();
    t_period_end = datePeriodEnd->date();

  }

  t_month = cbMonth->currentIndex() + 1;
  t_year = spinYear->value();
  
  QDialog::accept();
}

void SvReportSettingsDialog::setupUi(bool showReportType,
                                     bool showEmployees,
                                     bool showPeriod,
                                     bool showMonthYear)
{
    this->setObjectName(QStringLiteral("SvReportSettingsDialog"));
//    this->resize(395, 344);
    int height = 344;
  
    QIcon icon;
    icon.addFile(QStringLiteral(":/reports/icons/cologne-icons/ico/current-work.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    
    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    
    /** тип отчета **/
    gbReportTypes = new QGroupBox(this);
    gbReportTypes->setObjectName(QStringLiteral("gbReportTypes"));
    
    QFont font;
    font.setBold(true);
    gbReportTypes->setFont(font);
    
    hlayReportTypes = new QHBoxLayout(gbReportTypes);
    hlayReportTypes->setObjectName(QStringLiteral("hlayReportTypes"));
    
    cbReportTypes = new QComboBox(gbReportTypes);
    cbReportTypes->setObjectName(QStringLiteral("cbReportTypes"));
    QFont font1;
    font1.setBold(false);
    cbReportTypes->setFont(font1);

    hlayReportTypes->addWidget(cbReportTypes);
    
    /** выбор сотрудника **/
    gbEmployee = new QGroupBox(this);
    gbEmployee->setObjectName(QStringLiteral("gbEmployee"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(gbEmployee->sizePolicy().hasHeightForWidth());
    gbEmployee->setSizePolicy(sizePolicy);
    gbEmployee->setFont(font);
    
    vlayEmployee = new QVBoxLayout(gbEmployee);
    vlayEmployee->setObjectName(QStringLiteral("vlayEmployee"));
    
    /* все */
    rbAll = new QRadioButton(gbEmployee);
    rbAll->setObjectName(QStringLiteral("rbAll"));
    rbAll->setLayoutDirection(Qt::LeftToRight);
    rbAll->setChecked(true);
    
    /* отдел */
    hlayDepartment = new QHBoxLayout();
    hlayDepartment->setObjectName(QStringLiteral("hlayDepartment"));
    
    rbDepartment = new QRadioButton(gbEmployee);
    rbDepartment->setObjectName(QStringLiteral("rbDepartment"));
    rbDepartment->setEnabled(true);
    rbDepartment->setMinimumSize(QSize(90, 0));
    rbDepartment->setMaximumSize(QSize(90, 16777215));
    rbDepartment->setLayoutDirection(Qt::LeftToRight);

    cbDepartment = new QComboBox(gbEmployee);
    cbDepartment->setObjectName(QStringLiteral("cbDepartment"));
    cbDepartment->setEnabled(false);
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(cbDepartment->sizePolicy().hasHeightForWidth());
    cbDepartment->setSizePolicy(sizePolicy1);
    cbDepartment->setFont(font1);

    hlayDepartment->addWidget(rbDepartment);
    hlayDepartment->addWidget(cbDepartment);

    /* сотрудник */
    hlayEmployee = new QHBoxLayout();
    hlayEmployee->setObjectName(QStringLiteral("hlayEmployee"));
    rbEmployee = new QRadioButton(gbEmployee);
    rbEmployee->setObjectName(QStringLiteral("rbEmployee"));
    rbEmployee->setEnabled(true);
    rbEmployee->setMinimumSize(QSize(90, 0));
    rbEmployee->setMaximumSize(QSize(90, 16777215));
    rbEmployee->setLayoutDirection(Qt::LeftToRight);

    cbEmployee = new QComboBox(gbEmployee);
    cbEmployee->setObjectName(QStringLiteral("cbEmployee"));
    cbEmployee->setEnabled(false);
    sizePolicy1.setHeightForWidth(cbEmployee->sizePolicy().hasHeightForWidth());
    cbEmployee->setSizePolicy(sizePolicy1);
    cbEmployee->setFont(font1);

    hlayEmployee->addWidget(rbEmployee);
    hlayEmployee->addWidget(cbEmployee);
    
    vlayEmployee->addWidget(rbAll);
    vlayEmployee->addLayout(hlayDepartment);
    vlayEmployee->addLayout(hlayEmployee);

    /** выбор периода **/
    gbPeriod = new QGroupBox(this);
    gbPeriod->setObjectName(QStringLiteral("gbPeriod"));
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(gbPeriod->sizePolicy().hasHeightForWidth());
    gbPeriod->setSizePolicy(sizePolicy2);
    gbPeriod->setFont(font);
    
    vlayPeriod = new QVBoxLayout(gbPeriod);
    vlayPeriod->setObjectName(QStringLiteral("vlayPeriod"));
    
    /* текущйи месяц */
    rbCurrentMonth = new QRadioButton(gbPeriod);
    rbCurrentMonth->setObjectName(QStringLiteral("rbCurrentMonth"));
    rbCurrentMonth->setChecked(true);
   
    /* прошлый месяц */
    rbLastMonth = new QRadioButton(gbPeriod);
    rbLastMonth->setObjectName(QStringLiteral("rbLastMonth"));
    
    /* период */
    rbPeriod = new QRadioButton(gbPeriod);
    rbPeriod->setObjectName(QStringLiteral("rbPeriod"));

    hlayPeriod = new QHBoxLayout();
    hlayPeriod->setObjectName(QStringLiteral("hlayPeriod"));
    
    lblPeriodBegin = new QLabel(gbPeriod);
    lblPeriodBegin->setObjectName(QStringLiteral("lblPeriodBegin"));
    lblPeriodBegin->setEnabled(false);
    lblPeriodBegin->setMinimumSize(QSize(50, 0));
    lblPeriodBegin->setMaximumSize(QSize(50, 16777215));
    lblPeriodBegin->setFont(font1);
    lblPeriodBegin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    datePeriodBegin = new QDateEdit(gbPeriod);
    datePeriodBegin->setObjectName(QStringLiteral("datePeriodBegin"));
    datePeriodBegin->setEnabled(false);
    datePeriodBegin->setFont(font1);

    lblPeriodEnd = new QLabel(gbPeriod);
    lblPeriodEnd->setObjectName(QStringLiteral("lblPeriodEnd"));
    lblPeriodEnd->setEnabled(false);
    lblPeriodEnd->setMinimumSize(QSize(50, 0));
    lblPeriodEnd->setMaximumSize(QSize(50, 16777215));
    lblPeriodEnd->setFont(font1);
    lblPeriodEnd->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    datePeriodEnd = new QDateEdit(gbPeriod);
    datePeriodEnd->setObjectName(QStringLiteral("datePeriodEnd"));
    datePeriodEnd->setEnabled(false);
    datePeriodEnd->setFont(font1);

    hlayPeriod->addWidget(lblPeriodBegin);
    hlayPeriod->addWidget(datePeriodBegin);
    hlayPeriod->addWidget(lblPeriodEnd);
    hlayPeriod->addWidget(datePeriodEnd);

    vlayPeriod->addWidget(rbCurrentMonth);
    vlayPeriod->addWidget(rbLastMonth);
    vlayPeriod->addWidget(rbPeriod);
    vlayPeriod->addLayout(hlayPeriod);
    
    /* месяц и год */
    gbMonthYear = new QGroupBox(this);
    gbMonthYear->setObjectName(QStringLiteral("gbMonthYear"));
    sizePolicy2.setHeightForWidth(gbMonthYear->sizePolicy().hasHeightForWidth());
    gbMonthYear->setSizePolicy(sizePolicy2);
    gbMonthYear->setFont(font);
    
    hlayMonthYear = new QHBoxLayout(gbMonthYear);
    hlayMonthYear->setObjectName(QStringLiteral("hlayMonthYear"));
    
    lblMonth = new QLabel(gbMonthYear);
    lblMonth->setObjectName(QStringLiteral("lblMonth"));
    QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(lblMonth->sizePolicy().hasHeightForWidth());
    lblMonth->setSizePolicy(sizePolicy3);
    lblMonth->setMinimumSize(QSize(50, 0));
    lblMonth->setMaximumSize(QSize(50, 16777215));
    lblMonth->setFont(font1);
    lblMonth->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
   
    cbMonth = new QComboBox(gbMonthYear);
    cbMonth->setObjectName(QStringLiteral("cbMonth"));
    cbMonth->setFont(font1);
   
    lblYear = new QLabel(gbMonthYear);
    lblYear->setObjectName(QStringLiteral("lblYear"));
    sizePolicy3.setHeightForWidth(lblYear->sizePolicy().hasHeightForWidth());
    lblYear->setSizePolicy(sizePolicy3);
    lblYear->setMinimumSize(QSize(50, 0));
    lblYear->setMaximumSize(QSize(50, 16777215));
    lblYear->setFont(font1);
    lblYear->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
   
    spinYear = new QSpinBox(gbMonthYear);
    spinYear->setObjectName(QStringLiteral("spinYear"));
    QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(spinYear->sizePolicy().hasHeightForWidth());
    spinYear->setSizePolicy(sizePolicy4);
    spinYear->setMinimumSize(QSize(70, 0));
    spinYear->setFont(font1);
    spinYear->setMinimum(2000);
    spinYear->setMaximum(2030);
    spinYear->setValue(2015);
   
    hlayMonthYear->addWidget(lblMonth);
    hlayMonthYear->addWidget(cbMonth);
    hlayMonthYear->addWidget(lblYear);
    hlayMonthYear->addWidget(spinYear);
   

    /* кнопки */
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    
    spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    bnOk = new QPushButton(this);
    bnOk->setObjectName(QStringLiteral("bnOk"));

    bnCancel = new QPushButton(this);
    bnCancel->setObjectName(QStringLiteral("bnCancel"));

    horizontalLayout->addItem(spacer1);
    horizontalLayout->addWidget(bnOk);
    horizontalLayout->addWidget(bnCancel);

    /* расставляем уровни на форме */
    verticalLayout->addWidget(gbReportTypes);
    verticalLayout->addWidget(gbEmployee);
    verticalLayout->addWidget(gbPeriod);
    verticalLayout->addWidget(gbMonthYear);
    verticalLayout->addLayout(horizontalLayout);
    
    /* надписи */
    this->setWindowTitle(QApplication::translate("SvReportSettingsDialog", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270 \320\276\321\202\321\207\320\265\321\202\320\260", 0));
    gbReportTypes->setTitle(QApplication::translate("SvReportSettingsDialog", "\320\242\320\270\320\277 \320\276\321\202\321\207\320\265\321\202\320\260", 0));
    gbEmployee->setTitle(QApplication::translate("SvReportSettingsDialog", "\320\241\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272\320\270", 0));
    rbAll->setText(QApplication::translate("SvReportSettingsDialog", "\320\222\321\201\320\265", 0));
    rbDepartment->setText(QApplication::translate("SvReportSettingsDialog", "\320\236\321\202\320\264\320\265\320\273", 0));
    rbEmployee->setText(QApplication::translate("SvReportSettingsDialog", "\320\241\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272", 0));
    gbPeriod->setTitle(QApplication::translate("SvReportSettingsDialog", "\320\237\320\265\321\200\320\270\320\276\320\264", 0));
    rbCurrentMonth->setText(QApplication::translate("SvReportSettingsDialog", "\320\242\320\265\320\272\321\203\321\211\320\270\320\271 \320\274\320\265\321\201\321\217\321\206", 0));
    rbLastMonth->setText(QApplication::translate("SvReportSettingsDialog", "\320\237\321\200\320\276\321\210\320\273\321\213\320\271 \320\274\320\265\321\201\321\217\321\206", 0));
    rbPeriod->setText(QApplication::translate("SvReportSettingsDialog", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214", 0));
    lblPeriodBegin->setText(QApplication::translate("SvReportSettingsDialog", "\320\235\320\260\321\207\320\260\320\273\320\276", 0));
    lblPeriodEnd->setText(QApplication::translate("SvReportSettingsDialog", "\320\272\320\276\320\275\320\265\321\206", 0));
    
    gbMonthYear->setTitle(QApplication::translate("SvReportSettingsDialog", "\320\234\320\265\321\201\321\217\321\206 \320\270 \320\263\320\276\320\264", 0));
    lblMonth->setText(QApplication::translate("SvReportSettingsDialog", "\320\234\320\265\321\201\321\217\321\206", 0));
    cbMonth->clear();
    cbMonth->insertItems(0, QStringList()
     << QApplication::translate("SvReportSettingsDialog", "\320\257\320\275\320\262\320\260\321\200\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\244\320\265\320\262\321\200\320\260\320\273\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\234\320\260\321\200\321\202", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\220\320\277\321\200\320\265\320\273\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\234\320\260\320\271", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\230\321\216\320\275\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\230\321\216\320\273\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\220\320\262\320\263\321\203\321\201\321\202", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\241\320\265\320\275\321\202\321\217\320\261\321\200\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\236\320\272\321\202\321\217\320\261\321\200\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\235\320\276\321\217\320\261\321\200\321\214", 0)
     << QApplication::translate("SvReportSettingsDialog", "\320\224\320\265\320\272\320\260\320\261\321\200\321\214", 0)
    );
    lblYear->setText(QApplication::translate("SvReportSettingsDialog", "\320\223\320\276\320\264", 0));
    
    bnOk->setText(QApplication::translate("SvReportSettingsDialog", "\320\236\320\232", 0));
    bnCancel->setText(QApplication::translate("SvReportSettingsDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));

    /* скрываем ненужные элементы */
    gbReportTypes->setVisible(showReportType);
    gbEmployee->setVisible(showEmployees);
    gbPeriod->setVisible(showPeriod);
    gbMonthYear->setVisible(showMonthYear);
    
    height -= showReportType ? 0 : 53;
    height -= showEmployees ? 0 : 106;
    height -= showPeriod ? 0 : 124;
    height -= showMonthYear ? 0 : 55;
    
    this->resize(395, height);
} 
