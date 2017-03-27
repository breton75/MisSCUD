#ifndef SV_SELECTHOLIDAYDIALOG_H
#define SV_SELECTHOLIDAYDIALOG_H

#include <QDialog>
#include <QString>
#include <QtWidgets/QTableView>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QComboBox>
#include <QVBoxLayout>
#include <QTableView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QGroupBox>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "sv_holiday.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

class SvSelectHolidayDialog : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvSelectHolidayDialog(QWidget *parent,
                                   bool showEditDelete = false,
                                   bool showSelect = true);
    
    ~SvSelectHolidayDialog();
    
    int     t_id = -1;
    QString t_holiday_name = "";

  private slots:
    void accept() Q_DECL_OVERRIDE;
    
    void slotNewItem();
    void slotEditItem();
    void slotDeleteItem();
    
  private:
    QVBoxLayout *vlay;
    QGroupBox *gbItems;
    QVBoxLayout *vlayItems;
    QComboBox   *cbItemsList;
    QHBoxLayout *layoutButtons;
    QPushButton *bnNewItem;
    QPushButton *bnEditItem;
    QPushButton *bnDeleteItem;
    QSpacerItem *hSpacer;
    QPushButton *bnSelect;
    QPushButton *bnCancel;

    void loadItems();
    void setupUi();
    
};

#endif // SV_SELECTHOLIDAYDIALOG_H
