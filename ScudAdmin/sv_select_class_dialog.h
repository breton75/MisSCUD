#ifndef SV_SELECTCLASSDIALOG_H
#define SV_SELECTCLASSDIALOG_H

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

#include "sv_dev_class.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

class SvSelectClassDialog : public QDialog
{
    Q_OBJECT
    
  public:
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvSelectClassDialog(QWidget *parent,
                                    bool showEditDelete = false,
                                    bool showSelect = true);
    
    ~SvSelectClassDialog();
    
    int     t_id;
    QString t_class_name;
    QString t_libPath;
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

#endif // SV_SELECTCLASSDIALOG_H
