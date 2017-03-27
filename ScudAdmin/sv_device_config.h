#ifndef SV_DEVICECONFIGURATION_H
#define SV_DEVICECONFIGURATION_H

#include <QDialog>
#include <QByteArray>
#include <QBuffer>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QtEndian>
#include <QMessageBox>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QComboBox>

#include "../../Common/sv_pgdb.h"
#include "../../Common/sv_tcpserverclient.h"
#include "../../Common/log.h"

#include "../defs_misscud.h"

enum SettingsDataTypes
{
  sddServerIP = 0,
  sddServerPort
};

/* эти структуры ипользуются в контроллере СКУД */
#pragma pack(1)
typedef struct IP_s
{
  unsigned char IP[4];
  unsigned int Port;
  unsigned char MAC[6];
}IP_my;
#pragma pack(pop)

#pragma pack(1)
typedef struct IP_serv
{
  unsigned char IP[4];
  unsigned int Port;
  unsigned char MAC[6];
  unsigned int sensorType;
  IP_my ip_serv;
}IP_my_srv;
#pragma pack(pop)
/***********************************************/

struct DeviceSettings
{
  QString device_name = "";
  QString device_current_ip = "";
  quint16 device_current_port = 0;
  QString device_new_ip = "";
  quint16 device_new_port = 0;
  QString server_ip = "";
  quint16 server_port = 0;
};

class SvDeviceConfiguration : public QDialog
{
    Q_OBJECT
    
  public:
    explicit SvDeviceConfiguration(int deviceId);
    
    ~SvDeviceConfiguration();

    int device_id;
    QString t_device_name = "";
    QString t_device_current_ip = "";
    quint16 t_device_current_port = 0;
    QString t_device_new_ip = "";
    quint16 t_device_new_port = 0;
    QString t_server_ip = ""; 
    quint16 t_server_port = 0;
    
    void setCurrentData(QString ip, quint16 port);
    
  private slots:
    void accept() Q_DECL_OVERRIDE;
    void apply();
    void request();
    
  private:
    void setupUi();
    void setEnabledWidgets(bool enabled);
    bool checkInput(bool onlyCurrent = false);
    
    QList<int>archServerIdList;   
//    SvTcpClient* client;
    
    QVBoxLayout *vlayMain;
    QGroupBox   *gbDeviceSettings;
    QVBoxLayout *vlayDeviceSettings;
    QHBoxLayout *hlayDeviceCurrentIP;
    QLabel      *lblDeviceCurrentIP;
    QLineEdit   *editDeviceCurrentIP;
    QHBoxLayout *hlayDeviceCurrentPort;
    QLabel      *lblDeviceCurrentPort;
    QLineEdit   *editDeviceCurrentPort;
    QHBoxLayout *hlayDeviceNewIP;
    QLabel      *lblDeviceNewIP;
    QLineEdit   *editDeviceNewIP;
    QHBoxLayout *hlayDeviceNewPort;
    QLabel      *lblDeviceNewPort;
    QLineEdit   *editDeviceNewPort;
    QGroupBox   *gbServerSettings;
    QVBoxLayout *vlayServerSettings;
    QHBoxLayout *hlaySensorType;
    QLabel      *lblSensorType;
    QComboBox   *cbSensorType;
    QHBoxLayout *hlayServerIP;
    QLabel      *lblServerIP;
    QLineEdit   *editServerIP;
    QHBoxLayout *hlayServerPort;
    QLabel      *lblServerPort;
    QLineEdit   *editServerPort;
    QHBoxLayout *hlayRequest;
    QPushButton *bnRequest;
    QSpacerItem *spacer2;
    QHBoxLayout *hlayButtons;
    QPushButton *bnApply;
    QSpacerItem *spacer1;
//    QPushButton *bnSave;
    QPushButton *bnCancel;
    
};

#endif // SV_DEVICECONFIGURATION_H
