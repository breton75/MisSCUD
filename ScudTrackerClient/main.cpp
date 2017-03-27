#include "mainwindow.h"
#include <QApplication>

#include "../../Common/log.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_userlogon.h"

using namespace log_ns;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QString s = qApp->applicationDirPath() + "/" + qApp->applicationName() + ".ini";
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
  l->show();
  assign_log(l->get_log());
  l->setResult(-1);
  
  while (l->result() == -1) { QApplication::processEvents(); }

  if (l->result() != QDialog::Accepted)
  {
    a.exit(-1);
    return -1;
  }
  
  l->close();
  sett->~SvSettings();
  
  MainWindow w;
  assign_log(w.get_log());
  w.show();
  
  return a.exec();
}
