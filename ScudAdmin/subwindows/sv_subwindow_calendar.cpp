#include "sv_subwindow_calendar.h"

extern SvPGDB* PGDB;
extern int CURRENT_USER_ID;
extern SvSelectHolidayDialog* SELECTHOLIDAY_UI;

SvSubWindowCalendar* SUBWINDOW_CALENDAR;

SvSubWindowCalendar::SvSubWindowCalendar(QMdiArea *parent) : 
  QWidget(parent)
{
  this->setObjectName(QStringLiteral(SUBWINDOW_CALENDAR_NAME));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  this->current_year = QDate::currentDate().year();
  this->setWindowTitle("Производственный календарь");
  
  // задаем настройки для ОКНА !
  subw = parent->addSubWindow(this);
  subw->setWindowIcon(QIcon(":/tree/icons/cologne-icons/ico/date.ico"));
  subw->setAttribute(Qt::WA_DeleteOnClose);
  
  // читаем размер и положение окна
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s, this);
  subw->resize(sett->readValue(IniGroupName, "Size", QSize(400, 200)).toSize());
  subw->move(sett->readValue(IniGroupName, "Position", QPoint(30, 30)).toPoint());
  subw->setWindowState(Qt::WindowState(sett->readValue(IniGroupName, "WindowState", Qt::WindowNoState).toInt()));
 
  /** создаем тулбар **/
  toolbar = new QToolBar(this);
  
  /** создаем элементы панели **/
  lblYear = new QLabel("Год", toolbar);
  lblYear->setObjectName(QStringLiteral("lblYear"));
  lblYear->setMinimumWidth(60);
  lblYear->setMaximumWidth(60);
  lblYear->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  
  lblDateDescription = new QLabel("", toolbar);
  lblDateDescription->setObjectName(QStringLiteral("lblYear"));
  lblDateDescription->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lblDateDescription->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  
  spinYear = new QSpinBox(toolbar);
  spinYear->setObjectName(QStringLiteral("spinYear"));
  spinYear->setMinimumWidth(60);
  spinYear->setMaximumWidth(60);
  spinYear->setMaximum(2030);
  spinYear->setMinimum(2000);
  spinYear->setValue(current_year);
  connect(spinYear, SIGNAL(valueChanged(int)), this, SLOT(yearChanged(int)));
  
  
  /** создаем action Обновить **/
  QIcon icon = QIcon();
  icon.addFile(QStringLiteral(":/munich/icons/munich-icons/ico/blue/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionUpdate = new QAction(this);
  actionUpdate->setObjectName(QStringLiteral("actionUpdate"));
  actionUpdate->setIcon(icon);
  actionUpdate->setText("Обновить");
  connect(actionUpdate, SIGNAL(triggered()), this, SLOT(on_actionUpdate_triggered()));  
  
  /** добавляем элементы на тулбар **/
  toolbar->insertWidget(0, lblYear);
  toolbar->insertWidget(0, spinYear);
  toolbar->insertSeparator(0);
  toolbar->insertAction(0, this->actionUpdate);
  toolbar->insertSeparator(0);
  toolbar->insertWidget(0, lblDateDescription);
  
  /** создаем набор календарей **/
  hlay1lbl = new QHBoxLayout();
  hlay1lbl->setSpacing(12);
  hlay1 = new QHBoxLayout();
  hlay1->setSpacing(12);
  
  hlay2lbl = new QHBoxLayout();
  hlay2lbl->setSpacing(12);
  hlay2 = new QHBoxLayout();
  hlay2->setSpacing(12);
  
  hlay3lbl = new QHBoxLayout();
  hlay3lbl->setSpacing(12);
  hlay3 = new QHBoxLayout();
  hlay3->setSpacing(12);
  
  hlay4lbl = new QHBoxLayout();
  hlay4lbl->setSpacing(12);
  hlay4 = new QHBoxLayout();
  hlay4->setSpacing(12);
  
  for(int i = 1; i < 13; i++)
  {
    labels.insert(i, new QLabel(monthNames[i - 1], this));
    labels.value(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    labels.value(i)->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QFont fnt = labels.value(i)->font();
    fnt.setBold(true);
    labels.value(i)->setFont(fnt);
    labels.value(i)->setStyleSheet("color: rgb(0, 0, 255);");
    months.insert(i, new QCalendarWidget(this));
    months.value(i)->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
         
    if(i < 4)      { hlay1lbl->addWidget(labels.value(i)); hlay1->addWidget(months.value(i)); }
    else if(i < 7) { hlay2lbl->addWidget(labels.value(i)); hlay2->addWidget(months.value(i)); }
    else if(i < 10){ hlay3lbl->addWidget(labels.value(i)); hlay3->addWidget(months.value(i)); }
    else           { hlay4lbl->addWidget(labels.value(i)); hlay4->addWidget(months.value(i)); }
         
    months.value(i)->setGridVisible(true);
    months.value(i)->setDateEditEnabled(false);
//    months.value(i)->setUpdatesEnabled(true);
    months.value(i)->setSelectionMode(QCalendarWidget::SingleSelection);
    months.value(i)->setStyleSheet(QStringLiteral("alternate-background-color: rgb(220, 220, 220);"));
    months.value(i)->setNavigationBarVisible(false);
    
    connect(months.value(i), SIGNAL(activated(QDate)), this, SLOT(dateDoubleClicked(QDate)));
    connect(months.value(i), SIGNAL(clicked(QDate)), this, SLOT(dateClicked(QDate)));
    
  }
  this->on_actionUpdate_triggered();
  
    
  /** расставляем элементы на форме **/
  vlay = new QVBoxLayout(this);
  vlay->setParent(this);
  vlay->setSpacing(12);
//  vlay->setContentsMargins(2, 2, 2, 2);
  vlay->setObjectName(QStringLiteral("vlay"));
 
  vlay->addWidget(toolbar);
  vlay->addLayout(hlay1lbl);
  vlay->addLayout(hlay1);
  vlay->addLayout(hlay2lbl);
  vlay->addLayout(hlay2);
  vlay->addLayout(hlay3lbl);
  vlay->addLayout(hlay3);
  vlay->addLayout(hlay4lbl);
  vlay->addLayout(hlay4);

}

SvSubWindowCalendar::~SvSubWindowCalendar()
{
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";

  SvSettings *sett = new SvSettings(s, this);
  if(subw->windowState() != Qt::WindowNoState)
  {
    sett->writeValue(IniGroupName, "WindowState", subw->windowState());
  }
  else
  {
    sett->writeValue(IniGroupName, "Size", subw->size());
    sett->writeValue(IniGroupName, "Position", subw->pos());
    sett->writeValue(IniGroupName, "WindowState", Qt::WindowNoState);
  }
  sett->~SvSettings();
  
  this->deleteLater();
}


void SvSubWindowCalendar::on_actionUpdate_triggered()
{
//  showBusy("Обновление данных");
//  connect(this, SIGNAL(dataUpdated()), dlgBusy, SLOT(accept()));
  
  for(int i = 1; i < 13; i++)
  {
    months.value(i)->setMinimumDate(QDate::fromString(QString("1.%1.%2").arg(i).arg(current_year), "d.M.yyyy"));
    months.value(i)->setMaximumDate(QDate::fromString(QString("%1.%2.%3")
                                                      .arg(months.value(i)->minimumDate().daysInMonth())
                                                      .arg(i)
                                                      .arg(current_year), "d.M.yyyy"));
  }
  
//  QTextCharFormat dayFormat;
//  dayFormat.setForeground(Qt::yellow);
//  dayFormat.setBackground(Qt::red);
  
  QTextCharFormat dayFormat = QTextCharFormat(months.value(1)->weekdayTextFormat(Qt::Monday));
  dayFormat.setForeground(Qt::red);
  dayFormat.setFontWeight(75);
  
  QSqlQuery *q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString("select * from holidays_calendar "
                                                 "where holiday_date >= '%1' and holiday_date <= '%2'")
                                         .arg(months.value(1)->minimumDate().toString("dd/MM/yyyy"))
                                         .arg(months.value(12)->maximumDate().toString("dd/MM/yyyy")), q).type())
  {
    q->finish();
    return;
  }
  
  while(q->next())
    months.value(q->value("holiday_date").toDate().month())->setDateTextFormat(q->value("holiday_date").toDate(), dayFormat);

  q->finish();
  
//  emit dataUpdated();
  
  
}

void SvSubWindowCalendar::dateDoubleClicked(QDate date)
{
  SELECTHOLIDAY_UI = new SvSelectHolidayDialog(this);
  if(SELECTHOLIDAY_UI->exec() == QDialog::Accepted)
  {
    int id = SELECTHOLIDAY_UI->t_id;
  
    if(QSqlError::NoError != PGDB->execSQL(QString(SQL_UPDATE_HOLIDAY_CALENDAR)
                                           .arg(date.toString("dd/MM/yyyy"))
                                           .arg(id)).type())
      return;
    
    QTextCharFormat dayFormat = QTextCharFormat(months.value(1)->weekdayTextFormat(Qt::Monday));
    if(id != -1)
    {
      dayFormat.setForeground(Qt::red);
      dayFormat.setFontWeight(75);
    }
    months.value(date.month())->setDateTextFormat(date, dayFormat);
    
  }
  
  SELECTHOLIDAY_UI->~SvSelectHolidayDialog();

}

void SvSubWindowCalendar::dateClicked(QDate date)
{
  QString dayOfWeek;
  QString dayType;
  
  switch (date.dayOfWeek())
  {
    case 1: dayOfWeek = "Понедельник"; break;
    case 2: dayOfWeek = "Вторник"; break;
    case 3: dayOfWeek = "Среда"; break;
    case 4: dayOfWeek = "Четверг"; break;
    case 5: dayOfWeek = "Пятница"; break;
    case 6: dayOfWeek = "Суббота"; break;
    case 7: dayOfWeek = "Воскресенье"; break;
  }
  
  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_SELECT_HOLIDAY_CALENDAR_DATE).arg(date.toString("dd/MM/yyyy")), q).type())
  {
    q->finish();
    delete q;
    return;
  }

  if(q->next())
    dayType = QString("Праздничный день (%1)").arg(q->value("holiday_name").toString()); 

  else if((date.dayOfWeek() == 6) || (date.dayOfWeek() == 7))
    dayType = "Выходной день";

  else dayType = "Рабочий день";
  
  q->finish();
  delete q;
    
  lblDateDescription->setText(QString("%1, %2, %3")
                              .arg(date.toString("dd.MM.yyyy"))
                              .arg(dayOfWeek)
                              .arg(dayType));
}

void SvSubWindowCalendar::yearChanged(int year)
{
  this->current_year = year;
  on_actionUpdate_triggered();
}


void SvSubWindowCalendar::showBusy(QString text)
{
  dlgBusy = new QDialog(this, Qt::Window | Qt::FramelessWindowHint);
  dlgBusy->resize(300, 100);
  dlgBusy->setStyleSheet(QStringLiteral("background-color: rgb(90, 170, 255);"));
  connect(dlgBusy, SIGNAL(accepted()), dlgBusy, SLOT(deleteLater()));
  
  QVBoxLayout* vlay = new QVBoxLayout(dlgBusy);
  
  QGroupBox* gbox = new QGroupBox(dlgBusy);
  gbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gbox->setTitle("");
  
  QVBoxLayout* vlay2 = new QVBoxLayout(gbox);
  
  QLabel* lbl = new QLabel(gbox);
  lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  lbl->setText(text);
  
  vlay->addWidget(gbox);
  vlay2->addWidget(lbl);
  
  dlgBusy->setModal(true);
  dlgBusy->show();
}

