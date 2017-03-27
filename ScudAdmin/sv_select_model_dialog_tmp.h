#ifndef SV_SELECT_MODEL_DIALOG
#define SV_SELECT_MODEL_DIALOG

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

QT_BEGIN_NAMESPACE

class SvSelectModelDialog: public QDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hlayItems;
//    QLabel *lblItems;
//    QComboBox *cbItems;
    QHBoxLayout *hlayButtons;
    QSpacerItem *spacer1;
    QPushButton *bnOk;
    QPushButton *bnCancel;
    QPushButton *bnNewItem;
    QTreeView* treeItems;
    

    explicit SvSelectModelDialog(QObject* parent);
    
    ~SvSelectModelDialog() { this->deleteLater(); }
    
//    void accept() Q_DECL_OVERRIDE {  }
    
    
    void setupUi()
    {
//        if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("SelectModelDialog"));
        this->resize(300, 70);
        verticalLayout = new QVBoxLayout(this);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        hlayItems = new QHBoxLayout();
        hlayItems->setObjectName(QStringLiteral("hlayItems"));
        
//        lblItems = new QLabel(this);
//        lblItems->setObjectName(QStringLiteral("lblItems"));
//        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
//        sizePolicy.setHorizontalStretch(0);
//        sizePolicy.setVerticalStretch(0);
//        sizePolicy.setHeightForWidth(lblItems->sizePolicy().hasHeightForWidth());
//        lblItems->setSizePolicy(sizePolicy);
//        lblItems->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        hlayItems->addWidget(lblItems);

        treeItems = new QTreeView(this);
        treeItems->setObjectName(QStringLiteral("treeItems"));
//        cbItems = new QComboBox(this);
//        cbItems->setObjectName(QStringLiteral("cbItems"));

        hlayItems->addWidget(treeItems);


        verticalLayout->addLayout(hlayItems);

        hlayButtons = new QHBoxLayout();
        hlayButtons->setObjectName(QStringLiteral("hlayButtons"));
        

        bnNewItem = new QPushButton(this);
        bnNewItem->setObjectName(QStringLiteral("bnNewItem"));
        hlayButtons->addWidget(bnNewItem);
        
        spacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        hlayButtons->addItem(spacer1);

        bnOk = new QPushButton(this);
        bnOk->setObjectName(QStringLiteral("bnOk"));
        hlayButtons->addWidget(bnOk);

        bnCancel = new QPushButton(this);
        bnCancel->setObjectName(QStringLiteral("bnCancel"));
        hlayButtons->addWidget(bnCancel);

        verticalLayout->addLayout(hlayButtons);

        this->setWindowTitle(QApplication::translate("Dialog", "\320\241\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272", 0));
//        lblItems->setText(QApplication::translate("Dialog", "\320\241\320\276\321\202\321\200\321\203\320\264\320\275\320\270\320\272", 0));
        bnOk->setText(QApplication::translate("Dialog", "OK", 0));
        bnCancel->setText(QApplication::translate("Dialog", "\320\236\321\202\320\274\320\265\320\275\320\260", 0));
        bnCancel->setText(QApplication::translate("Dialog", "Создать");

        QMetaObject::connectSlotsByName(this);
        
    } // setupUi
};

//namespace Ui {
//    class Dialog: public Ui_Dialog {};
//} // namespace Ui

QT_END_NAMESPACE


#endif // SV_SELECT_MODEL_DIALOG

