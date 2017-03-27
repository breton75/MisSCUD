#ifndef DEFS_H
#define DEFS_H

#include <QStandardItem>

enum ItemTypes
{
  itEmployees = 65537,
  itEmployee,
  itEmployeeId,
  itEmployeeName,
  itEmployeePosition,
  itEmployeeLoadAllot,
  itEmployeeLoadFact,
  itEmployeeLogin,
  itEmployeeDepartment,
  itEmployeeWorkTime,
  itId,
  itDescription,
  itDevices,
  itDevice,
  itDeviceModel,
  itDeviceClass,
  itDeviceBrand,
  itVideo,
  itController,
  itSensor,
  itScud,
  itScudKeys,
  itScudKey,
  itScudReport,
  itScudSensorNumber,
  itScudDateTime,
  itScudDeviceName,
  itDepartments,
  itDepartment,
  itReports,
  itReport,
  itTimeAccounting, 
  itAbsenceJournal,
  itErrandJournal,  
  itBusinessCalendar,
  itTimeSheet,
  itUndefined
};

enum DataRoles
{
  roleId = Qt::UserRole + 1,
  roleParentId,
  roleState,
  roleFieldName
};

/** определяем типы элементов дерева **/
class SvDevicesItem: public QStandardItem
{
  public:
    explicit SvDevicesItem(const QString &text) { setText(text); }
    int type() const { return itDevices; }
};
class SvDeviceItem: public QStandardItem
{
  public:
    explicit SvDeviceItem(const QString &text) { setText(text); }
    SvDeviceItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itDevice; }
};
class SvDeviceModelItem: public QStandardItem
{
  public:
    explicit SvDeviceModelItem(const QString &text) { setText(text); }
    SvDeviceModelItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); } 
    int type() const { return itDeviceModel; }
};
class SvDeviceClassItem: public QStandardItem
{
  public:
    explicit SvDeviceClassItem(const QString &text) { setText(text); }
    SvDeviceClassItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); } 
    int type() const { return itDeviceClass; }
};
class SvSCUDItem: public QStandardItem
{
  public:
    explicit SvSCUDItem(const QString &text) { setText(text); }
    int type() const { return itScud; }
};
class SvVideoItem: public QStandardItem
{
  public:
    explicit SvVideoItem(const QString &text) { setText(text); }
    int type() const { return itVideo; }
};


class SvEmployeesItem: public QStandardItem
{
  public:
    explicit SvEmployeesItem(const QString &text) { setText(text); }
    SvEmployeesItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itEmployees; }
};
class SvEmployeeItem: public QStandardItem
{
  public:
    explicit SvEmployeeItem(const QString &text) { setText(text); }
    SvEmployeeItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itEmployee; }
};
class SvDepartmentItem: public QStandardItem
{
  public:
    explicit SvDepartmentItem(const QString &text) { setText(text); }
    SvDepartmentItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itDepartment; }
};


class SvScudKeys: public QStandardItem
{
  public:
    explicit SvScudKeys(const QString &text) { setText(text); }
    SvScudKeys(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itScudKeys; }
};

/* отчеты */
class SvReportsItem: public QStandardItem
{
  public:
    explicit SvReportsItem(const QString &text) { setText(text); }
    SvReportsItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itReports; }
};
class SvReportItem: public QStandardItem
{
  public:
    explicit SvReportItem(const QString &text) { setText(text); }
    SvReportItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itReport; }
};

/* учет рабочего времени */
class SvTimeAccountingItem: public QStandardItem
{
  public:
    explicit SvTimeAccountingItem(const QString &text) { setText(text); }
    SvTimeAccountingItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itTimeAccounting; }
};
class SvAbsenceJournalItem: public QStandardItem
{
  public:
    explicit SvAbsenceJournalItem(const QString &text) { setText(text); }
    SvAbsenceJournalItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itAbsenceJournal; }
};
class SvErrandJournalItem: public QStandardItem
{
  public:
    explicit SvErrandJournalItem(const QString &text) { setText(text); }
    SvErrandJournalItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itErrandJournal; }
};
class SvBusinessCalendarItem: public QStandardItem
{
  public:
    explicit SvBusinessCalendarItem(const QString &text) { setText(text); }
    SvBusinessCalendarItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itBusinessCalendar; }
};
class SvTimeSheetItem: public QStandardItem
{
  public:
    explicit SvTimeSheetItem(const QString &text) { setText(text); }
    SvTimeSheetItem(const QIcon &icon, const QString &text) { setIcon(icon); setText(text); }
    int type() const { return itTimeSheet; }
};

#define CR "\n"

/* devices */
#define SQL_NEW_DEVICE "select "\
"  new_device_id "\
"from new_device('%1', %2, %3, %4, %5, %6, '%7')"

#define SQL_UPDATE_DEVICE "select "\
"update_device('%1', %2, %3, %4, %5, %6, '%8', %9)"

#define SQL_SELECT_DEVICE "SELECT " CR \
"  devices.id as device_id, " CR \
"  devices.device_name, " CR \
"  devices.ip, " CR \
"  devices.port, " CR \
"  devices.description, " CR \
"  devices.model_id, " CR \
"  devices.class_id, " CR \
"  devices.brand_id, " CR \
"  models.model_name, " CR \
"  models.model_lib_path, " CR \
"  classes.class_name, " CR \
"  classes.class_lib_path, " CR \
"  brands.brand_name " CR \
"FROM " CR \
"  public.devices " CR \
"  left join public.models on devices.model_id = models.id " CR \
"  left join public.classes on devices.class_id = classes.id " CR \
"  left join public.brands on devices.brand_id = brands.id " CR \
"where devices.id = %1"

#define SQL_SELECT_DEVICES_LIST "SELECT " CR \
"  devices.id as device_id, " CR \
"  devices.device_name, " CR \
"  devices.ip, " CR \
"  devices.port, " CR \
"  devices.description, " CR \
"  devices.model_id, " CR \
"  devices.class_id, " CR \
"  devices.brand_id, " CR \
"  models.model_name, " CR \
"  models.model_lib_path, " CR \
"  classes.class_name, " CR \
"  classes.class_lib_path, " CR \
"  brands.brand_name " CR \
"FROM " CR \
"  public.devices " CR \
"  left join public.models on devices.model_id = models.id " CR \
"  left join public.classes on devices.class_id = classes.id " CR \
"  left join public.brands on devices.brand_id = brands.id " CR \
" %1 " CR\
"ORDER BY model_name, device_name ASC"

#define SQL_CHECK_IP_EXISTS "select cnt from check_ip_exists(%1)"


/* models */
#define SQL_NEW_MODEL "select "\
"  new_model_id "\
"from new_model('%1', %2, %3, '%4', '%5')"

#define SQL_UPDATE_MODEL "select "\
"update_model('%1', %2, %3, '%4', '%5', %6)"

#define SQL_SELECT_MODEL "SELECT " CR\
"  models.id as model_id, " CR\
"  models.model_name, " CR\
"  models.model_lib_path, " CR\
"  models.class_id, " CR\
"  models.brand_id, " CR\
"  models.description, " CR\
"  classes.class_name, " CR\
"  classes.class_lib_path, " CR\
"  brands.brand_name " CR\
"FROM " CR\
"  public.models " CR\
"  left join public.classes on models.class_id = classes.id " CR\
"  left join public.brands on models.brand_id = brands.id " CR\
"where models.id = %1"

#define SQL_SELECT_MODELS_LIST \
"SELECT " CR\
"  models.id as model_id, " CR\
"  models.model_name, " CR\
"  models.model_lib_path, " CR\
"  models.class_id, " CR\
"  models.brand_id, " CR\
"  models.description, " CR\
"  classes.class_name, " CR\
"  classes.class_lib_path, " CR\
"  brands.brand_name " CR\
"FROM " CR\
"  public.models " CR\
"  left join public.classes on models.class_id = classes.id " CR\
"  left join public.brands on models.brand_id = brands.id " CR\
" %1 " CR\
"ORDER BY brand_name, model_name ASC"

/* brands */
#define SQL_NEW_BRAND "select "\
"  new_brand_id "\
"from new_brand('%1', '%2')"

#define SQL_UPDATE_BRAND "select "\
"update_brand('%1', '%2', %3)"

#define SQL_SELECT_BRAND "SELECT " CR\
"  brands.id as brand_id, " CR\
"  brands.brand_name, " CR\
"  brands.description " CR\
"FROM " CR\
"  public.brands " CR\
"where brands.id = %1"


/* classes */
#define SQL_NEW_CLASS "select "\
"  new_class_id "\
"from new_class('%1', '%2', ?, '%3')"

#define SQL_UPDATE_CLASS "select "\
"update_class('%1', '%2', ?, '%3', %4)"

#define SQL_SELECT_CLASS "SELECT " CR\
"  classes.id as class_id, " CR\
"  classes.class_name, " CR\
"  classes.class_lib_path, " CR\
"  classes.icon, " CR\
"  classes.description " CR\
"FROM " CR\
"  public.classes " CR\
"where classes.id = %1"

#define SQL_SELECT_CLASSES_LIST "SELECT " CR\
"  classes.id as class_id, " CR\
"  classes.class_name, " CR\
"  classes.class_lib_path, " CR\
"  classes.icon, " CR\
"  classes.description " CR\
"FROM " CR\
"  public.classes " CR\
" %1 " CR\
"ORDER BY class_name ASC"


/* employees */
#define SQL_NEW_EMPLOYEE "select "\
"  new_employee_id "\
"from new_employee('%1', '%2', '%3', %4, '%5', '%6', '%7', '%8', '%9', ?, '%10', '%11')"

#define SQL_UPDATE_EMPLOYEE "select "\
"update_employee('%1', '%2', '%3', %4, '%5', '%6', '%7', '%8', '%9', ?, '%10', '%11', %12)"

#define SQL_SELECT_EMPLOYEE "SELECT " CR\
"  employees.id as employee_id, " CR\
"  employees.employee_name, " CR\
"  employees.employee_patronymic, " CR\
"  employees.employee_surename, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.birth_date, " CR\
"  employees.department_id, " CR\
"  employees.employee_position, " CR\
"  employees.email, " CR\
"  employees.phone1, " CR\
"  employees.phone2, " CR\
"  employees.description, " CR\
"  employees.foto, " CR\
"  departments.department_name " CR\
"FROM employees " CR\
"LEFT JOIN departments ON employees.department_id = departments.id " CR\
"WHERE employees.id = %1"


#define SQL_SELECT_EMPLOYEES_LIST "SELECT " CR\
"  employees.id as employee_id, " CR\
"  employees.employee_name, " CR\
"  employees.employee_patronymic, " CR\
"  employees.employee_surename, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.birth_date, " CR\
"  employees.department_id, " CR\
"  employees.employee_position, " CR\
"  employees.email, " CR\
"  employees.phone1, " CR\
"  employees.phone2, " CR\
"  employees.description, " CR\
"  employees.foto, " CR\
"  departments.department_name " CR\
"FROM employees " CR\
"LEFT JOIN departments ON employees.department_id = departments.id " CR\
" %1 " \
"ORDER BY departments.department_name, employees.employee_surename ASC"

#define SQL_SELECT_EMPLOYEES_LIST_LIGHT "SELECT " CR\
"  employees.id as employee_id, " CR\
"  employees.employee_position, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.department_id " CR\
"FROM employees " CR\
" %1 " \
"ORDER BY employees.employee_surename ASC"

/* scud keys */
#define SQL_NEW_SCUDKEY "select "\
"  new_scudkey_id "\
"from new_scudkey(%1, %2, '%3')"

#define SQL_UPDATE_SCUDKEY "select "\
"update_scudkey(%1, %2, '%3')"

#define SQL_SELECT_SCUDKEY "SELECT scuds.id as scud_id, " CR\
"  scuds.key_value, " CR\
"  scuds.employee_id, " CR\
"  employees.employee_name, " CR\
"  employees.employee_patronymic, " CR\
"  employees.employee_surename, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.employee_position as employee_position, " CR\
"  scuds.description " CR\
"FROM scuds " CR\
"LEFT JOIN employees on scuds.employee_id = employees.id" CR\
"WHERE scuds.id = %1 "

#define SQL_SELECT_SCUDKEYS_LIST "SELECT scuds.id as scud_id, " CR\
"  scuds.key_value, " CR\
"  scuds.employee_id, " CR\
"  employees.employee_name, " CR\
"  employees.employee_patronymic, " CR\
"  employees.employee_surename, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.employee_position as employee_position, " CR\
"  scuds.description " CR\
"FROM scuds " CR\
"LEFT JOIN employees on scuds.employee_id = employees.id" CR\
" %1 " CR\
"ORDER BY key_value ASC"

#define SQL_CHECK_KEY_EXISTS "SELECT key_count from check_key_exists(%1)"

/* departments */
#define SQL_NEW_DEPARTMENT "select "\
"  new_department_id "\
"from new_department('%1', %2, ?, '%3')"

#define SQL_UPDATE_DEPARTMENT "select "\
"update_department('%1', %2, ?, '%3', %4)"

#define SQL_SELECT_DEPARTMENT "SELECT " CR\
"  departments.id as department_id, " CR\
"  departments.department_name, " CR\
"  departments.boss_id, " CR\
"  employees.employee_name, " CR\
"  employees.employee_patronymic, " CR\
"  employees.employee_surename, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.employee_position as employee_position, " CR\
"  departments.icon, " CR\
"  departments.description " CR\
"FROM " CR\
"  public.departments " CR\
"  LEFT JOIN employees ON departments.boss_id = employees.id " CR\
"where departments.id = %1"

#define SQL_SELECT_DEPARTMENTS_LIST "SELECT " CR\
"  departments.id as department_id, " CR\
"  departments.department_name, " CR\
"  departments.boss_id, " CR\
"  employees.employee_name, " CR\
"  employees.employee_patronymic, " CR\
"  employees.employee_surename, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.employee_position as employee_position, " CR\
"  departments.icon, " CR\
"  departments.description " CR\
"FROM " CR\
"  public.departments " CR\
"  LEFT JOIN employees ON departments.boss_id = employees.id " CR\
" %1 " CR\
"ORDER BY department_name ASC"


/** отчеты **/
/* general */
#define SQL_REPORT_GENERAL "SELECT * " CR\
"FROM report_general(%1, %2, '%3', '%4', %5)"

/* absence journal */
#define SQL_REPORT_ABSENCE_JOURNAL "SELECT * " CR\
"FROM report_absence_journal(%1, %2, '%3', '%4')"

/* errand journal */
#define SQL_REPORT_ERRAND_JOURNAL "SELECT * " CR\
"FROM report_errand_journal(%1, %2, '%3', '%4')"

/* time sheet */
#define SQL_REPORT_TIMESHEET "SELECT * " CR\
"FROM report_timesheet(%1, %2, %3, %4)"

#define SQL_SELECT_REPORT_TYPES "SELECT " CR\
"  id as report_type_id, " CR\
"  report_type_name " CR\
"FROM report_types %1"

/* tracker */
#define SQL_NEW_PASSAGE "select new_passage(%1, %2, %3, '%4')"

#define SQL_NEW_PASSAGE_TRASH "select new_passage_trash(%1, %2, %3, '%4', %5)"

#define SQL_SELECT_TRACKING_DEVICES_LIST "SELECT " CR \
"  device_trackers.device_id, " CR\
"  device_tracker_period.period, " CR \
"  device_tracker_period.response_timeout, " CR \
"  device_tracker_period.track_method, " CR \
"  device_tracker_period.alive, " CR \
"  devices.id as device_id, " CR \
"  devices.device_name, " CR \
"  devices.ip, " CR \
"  devices.port, " CR \
"  devices.description, " CR \
"  devices.model_id, " CR \
"  devices.class_id, " CR \
"  devices.brand_id, " CR \
"  models.model_name, " CR \
"  models.model_lib_path, " CR \
"  classes.class_name, " CR \
"  classes.class_lib_path, " CR \
"  brands.brand_name " CR \
"FROM " CR \
"  public.tracker_period " CR \
"  left join public.devices on devices.id = tracker_period.device_id " CR \
"  left join public.models on devices.model_id = models.id " CR \
"  left join public.classes on devices.class_id = classes.id " CR \
"  left join public.brands on devices.brand_id = brands.id " CR \
" %1 " CR\
"ORDER BY device_name, model_name ASC"


/* holidays */
#define SQL_NEW_HOLIDAY "select "\
"  new_holiday_id "\
"from new_holiday('%1')"

#define SQL_UPDATE_HOLIDAY "select "\
"update_holiday('%1', %2)"

#define SQL_SELECT_HOLIDAY "SELECT " CR\
"  holidays.id as holiday_id, " CR\
"  holidays.holiday_name " CR\
"FROM " CR\
"  public.holidays " CR\
"where holidays.id = %1"

#define SQL_SELECT_HOLIDAYS_LIST "SELECT " CR\
"  holidays.id as holiday_id, " CR\
"  holidays.holiday_name " CR\
"FROM " CR\
"  public.holidays " CR\
" %1 " CR\
"ORDER BY holiday_name ASC"

/* holidays calendar */
#define SQL_UPDATE_HOLIDAY_CALENDAR "SELECT " \
"update_holidays_calendar('%1', %2)"

#define SQL_SELECT_HOLIDAY_CALENDAR_DATE "SELECT " CR\
"  holidays_calendar.holiday_date, " CR\
"  holidays.holiday_name " CR\
"FROM " CR\
"  public.holidays_calendar " CR\
"  LEFT JOIN public.holidays ON holidays.id = holidays_calendar.holiday_id " CR\
"WHERE " CR\
"  holidays_calendar.holiday_date = '%1'"


/* absences */
#define SQL_SELECT_ABSENCES_LIST "SELECT " CR\
"  absences.id as absence_id, " CR\
"  absences.absence_name, " CR\
"  absences.designation " CR\
"FROM absences " CR\
"%1 " CR\
"ORDER BY absences.absence_name ASC"


/* absence journal */
#define SQL_NEW_ABSENCE_JOURNAL "SELECT " \
"  new_absence_journal_id FROM new_absence_journal(%1, %2, '%3', '%4')"

#define SQL_UPDATE_ABSENCE_JOURNAL "SELECT " \
" update_absence_journal(%1, %2, '%3', '%4', %5)"

#define SQL_SELECT_ABSENCE_JOURNAL "SELECT " CR\
"  absence_journal.id as absence_journal_id, " CR\
"  absence_journal.absence_begin, " CR\
"  absence_journal.absence_end, " CR\
"  absences.id as absence_id, " CR\
"  absences.absence_name, " CR\
"  employees.id as employee_id, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.employee_position as employee_position " CR\
"FROM absence_journal " CR\
"LEFT JOIN absences ON absences.id = absence_journal.absence_id " CR\
"LEFT JOIN employees ON employees.id = absence_journal.employee_id " CR\
"WHERE absence_journal.id = %1"

/* errand journal */
#define SQL_NEW_ERRAND_JOURNAL "SELECT " \
"  new_errand_journal_id FROM new_errand_journal(%1, '%2', '%3', '%4', '%5')"

#define SQL_UPDATE_ERRAND_JOURNAL "SELECT " \
" update_errand_journal(%1, '%2', '%3', '%4', '%5', %6)"

#define SQL_SELECT_ERRAND_JOURNAL "SELECT " CR\
"  errand_journal.id as errand_journal_id, " CR\
"  errand_journal.errand_begin, " CR\
"  errand_journal.errand_end, " CR\
"  errand_journal.destination, " CR\
"  errand_journal.description, " CR\
"  employees.id as employee_id, " CR\
"  concat(employees.employee_surename, ' ', " CR\
"         substr(employees.employee_name, 1, 1), '. ', " CR\
"         substr(employees.employee_patronymic, 1, 1), '. ') as short_name, " CR\
"  employees.employee_position as employee_position " CR\
"FROM errand_journal " CR\
"LEFT JOIN employees ON employees.id = errand_journal.employee_id " CR\
"WHERE errand_journal.id = %1 ORDER BY errand_begin, errand_end ASC"

#endif
