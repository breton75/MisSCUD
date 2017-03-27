#ifndef SV_SELECT_MODEL_DIALOG
#define SV_SELECT_MODEL_DIALOG

#include <QDialog>
#include <QString>
#include <QStandardItemModel>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QMessageBox>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QTreeView>

#include "sv_dev_model.h"
#include "../../Common/sv_settings.h"
#include "../../Common/sv_pgdb.h"

class SvTreeModelsList;

class SvSelectModelDialog : public QDialog
{
    Q_OBJECT
    
  public:
    
    enum ShowMode { smNew = 0, smEdit = 1 };
                    
    explicit SvSelectModelDialog(QWidget *parent,
                             bool showEditDelete = false,
                             bool showSelect = true);
    
    ~SvSelectModelDialog();
    
    int     t_id;
    int     t_class_id;
    int     t_brand_id;
    QString t_model_name;
    QString t_class_name;
    QString t_brand_name;
    QString t_driver_path;
    QString t_description;
    
  private slots:
    void accept() Q_DECL_OVERRIDE;
    void currentChanged(const QModelIndex & current);
    
    void on_bnNewItem_clicked();
    void on_bnEditItem_clicked();
    void on_bnDeleteItem_clicked();
    
  private:
    QVBoxLayout *verticalLayout;
    QLabel *lblItems;
    SvTreeModelsList* treeItems;
    QHBoxLayout *hlayButtons;
    QPushButton *bnNewItem;
    QPushButton *bnEditItem;
    QPushButton *bnDeleteItem;
    QSpacerItem *spacer1;
    QPushButton *bnSelect;
    QPushButton *bnCancel;
    
    void setupUi();
    void loadItems(bool reload = false, int setToId = -1);
    
};

class SvTreeModelsList : public QTreeView
{
  Q_OBJECT
  
  public:
    explicit SvTreeModelsList(QWidget* parent);
    
    ~SvTreeModelsList();
    
    QStandardItemModel* model = new QStandardItemModel();
    QStandardItem *itemRoot = model->invisibleRootItem();
    
  private:
//    void contextMenuEvent(QContextMenuEvent * event) Q_DECL_OVERRIDE ;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
//    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void currentChanged(const QModelIndex & current, const QModelIndex & previous) Q_DECL_OVERRIDE;

  signals:
    void currentChanged(const QModelIndex & current);
    void mouseDoubleClick();
    
};


#endif // SV_SELECT_MODEL_DIALOG
