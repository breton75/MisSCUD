#ifndef SV_SELECTBRANDDIALOG_H
#define SV_SELECTBRANDDIALOG_H

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
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "sv_dev_brand.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

class SvSelectBrandDialog : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvSelectBrandDialog(QWidget *parent,
                                   bool showEditDelete = false,
                                   bool showSelect = true);
    
    ~SvSelectBrandDialog();
    
    int     t_id = -1;
    QString t_brand_name = "";
    QString t_address = "";
    QString t_email = "";
    QString t_site = "";
    QString t_description = "";

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

#endif // SV_SELECTBRANDDIALOG_H
