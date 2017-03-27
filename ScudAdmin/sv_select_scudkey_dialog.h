#ifndef SV_SELECTSCUDKEYDIALOG_H
#define SV_SELECTSCUDKEYDIALOG_H

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

#include "sv_scud.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

class SvSelectScudkeyDialog : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvSelectScudkeyDialog(QWidget *parent,
                                   bool showCreate = true,
                                   bool showEditDelete = false);
    
    ~SvSelectScudkeyDialog();
    
    int     t_id;
    QString t_key_value;
    QString t_description;
    
  private slots:
    void accept() Q_DECL_OVERRIDE;
    
    void slotNewItem();
    void slotEditItem();
    void slotDeleteItem();
    
  private:
    QVBoxLayout *vlay;
    QGroupBox   *gbItems;
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

#endif // SV_SELECTSCUDKEYDIALOG_H
