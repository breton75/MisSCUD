#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

#include "../../Common/sv_userlogon.h"
#include "../../Common/log.h"
#include "../../Common/sv_settings.h"

using namespace log_ns;

extern SvPGDB* PGDB;
extern struct_user_info USER_INFO;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  
  QString s = QApplication::applicationDirPath() + "/" + QApplication::applicationName() + ".ini";
  SvSettings *sett = new SvSettings(s);
  
  SvUserLogon *l = new SvUserLogon(0,
                                  sett->readValue("LOGON", "ShowHost", true).toBool(),
                                  sett->readValue("LOGON", "ShowPort", false).toBool(),
                                  sett->readValue("LOGON", "ShowDBName", false).toBool(),
                                  sett->readValue("LOGON", "ShowUser", true).toBool(),
                                  sett->readValue("LOGON", "ShowPass", true).toBool(),
                                  sett->readValue("LOGON", "ShowLog", false).toBool(),
                                  sett->readValue("LOGON", "Host", "169.254.1.1").toString(),
                                  sett->readValue("LOGON", "Port", 5432).toInt(),
                                  sett->readValue("LOGON", "DBName", "PSSDB").toString(),
                                  sett->readValue("LOGON", "User", "").toString(),
                                  sett->readValue("LOGON", "Pass", "").toString());
//  l->show();
  assign_log(l->get_log());
//  l->setResult(-1);
  
  QString login;
  QString pass;
  
//  l->show();
  if(l->exec() == QDialog::Accepted)
  {
//    qDebug() << l->windowTitle();
//    qDebug() << l->t_user;
//    pass = l->t_pass;

  }
  else
  {
    l->close();
    a.exit(-1);
//    return -1;
  }
  l->close();
  /* **/

  
//  QSqlQuery* q = new QSqlQuery(PGDB->db);
//  QSqlError err = PGDB->execSQL(QString("select * from get_user_info('%1', '%2')")
//                                .arg(login)
//                                .arg(pass), q);
//  if(QSqlError::NoError == err.type())
//  {
//    q->finish();
//    return -1;
//  }
  
//  bool found = q->next() & (!q->value("id").isNull());
//  if(found)
//  {
//    USER_INFO.id = q->value("id").toInt();
//    USER_INFO.employee_id = q->value("employee_id").toInt();
    
//    QByteArray b = q->value("rights").toByteArray();
//    USER_INFO.allowAdd = bool(b.at(0));
//    USER_INFO.allowEdit = bool(b.at(1));
//    USER_INFO.allowDelete = bool(b.at(2));
//    USER_INFO.allowReports = bool(b.at(3));
    
//    USER_INFO.viewDevices = bool(b.at(4));
//    USER_INFO.viewEmployees = bool(b.at(5));
//    USER_INFO.viewUsers = bool(b.at(6));
    
//    sett->writeValue("LOGON", "User", l->t_user);  
//  }
//  q->finish();
//  free(q);
  sett->~SvSettings();
  
//  if(!found)
//  {
//    log(m_Critical, QString("Пользователь %1 не найден").arg(l->t_user));
//    l->close();
//    return -1;
//  }
    
  
  
  /*  ***/
  
  MainWindow w;
  assign_log(w.get_log());
  w.show();
  
  w.AfterShow();
  
  return a.exec();
}
