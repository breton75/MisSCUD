#ifndef SVSUBWINDOWCALENDAR_H
#define SVSUBWINDOWCALENDAR_H

#include <QDebug>
#include <QWidget>
#include <QMdiArea>
#include <QAction>
#include <QMap>
#include <QTextCharFormat>

#include <QVBoxLayout>
#include <QLabel>
#include <QMdiSubWindow>
#include <QApplication>
#include <QToolBar>
#include <QCalendarWidget>
#include <QSpinBox>

#include <QSqlQuery>
#include <QSqlError>

#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_settings.h"
#include "sv_select_holiday_dialog.h"

#include "../defs_misscud.h"

#define SUBWINDOW_CALENDAR_NAME "subw_window_calendar"

class SvSubWindowCalendar : public QWidget
{
    Q_OBJECT
  public:
    explicit SvSubWindowCalendar(QMdiArea *parent);
    ~SvSubWindowCalendar();
  
  private:
    QMdiSubWindow *subw;
    
    QLabel* lblYear;
    QLabel* lblDateDescription;
    QSpinBox* spinYear;
    
    QLabel* lblJan;
    QLabel* lblFeb;
    QLabel* lblMar;
    QLabel* lblApr;
    QLabel* lblMay;
    QLabel* lblJun;
    QLabel* lblJul;
    QLabel* lblAug;
    QLabel* lblSep;
    QLabel* lblOct;
    QLabel* lblNov;
    QLabel* lblDec;
    
    QVBoxLayout* vlay;
    QHBoxLayout* hlay1lbl;
    QHBoxLayout* hlay1;
    QHBoxLayout* hlay2lbl;
    QHBoxLayout* hlay2;
    QHBoxLayout* hlay3lbl;
    QHBoxLayout* hlay3;
    QHBoxLayout* hlay4lbl;
    QHBoxLayout* hlay4;
    QToolBar* toolbar;
    
    int current_year;
    QMap<int, QCalendarWidget*> months;
    QMap<int, QLabel*> labels;
    
    QAction *actionUpdate;
    
    QString IniGroupName = "BUSINESS CALENDAR";
    QDialog* dlgBusy;
    
    QString monthNames[12] = {"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль",
                              "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"};

  private slots:
    void on_actionUpdate_triggered();
    void dateDoubleClicked(QDate date);
    void dateClicked(QDate date);
    void yearChanged(int year);
    
    void showBusy(QString text);
    
  signals:
    void dataUpdated();
};

#endif // SVSUBWINDOWCALENDAR_H
