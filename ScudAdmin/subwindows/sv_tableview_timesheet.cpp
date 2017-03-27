#include "sv_tableview_timesheet.h"

using namespace log_ns;

extern SvPGDB* PGDB;
extern int CURRENT_USER_ID;
extern SvSubWindowMain* SUBWINDOW_MAIN_UI;
extern SvSubWindowAbsenceJournal* SUBWINDOW_ABSENCE_JOURNAL_UI;
extern SvSubWindowErrandJournal* SUBWINDOW_ERRAND_JOURNAL_UI;

SvTableViewTimeSheet::SvTableViewTimeSheet(QWidget *parent,
                               int departmentId,
                               int employeeId,
                               int month,
                               int year,
                               QMdiArea *mdiWindow)
//  QWidget(parent)
{
  this->setParent(parent);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  this->mdiWindow = mdiWindow;
  
  this->departmentId = departmentId;
  this->employeeId = employeeId;
  this->month = month;
  this->year = year;
  
  /** *** загружаем данные *** **/
  this->setupModelData();
  this->setShowGrid(true);
  
  QIcon icon;
  QFont fnt;
  fnt.setBold(true);
  
  /** *********** actions ************** **/
  icon.addFile(QStringLiteral(":/absence_journal/icons/cologne-icons/ico/refresh.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionReload = new QAction(this);
  actionReload->setObjectName(QStringLiteral("actionReload"));
  actionReload->setIcon(icon);
  actionReload->setText("Обновить"); 
  actionReload->setEnabled(true);
  
  icon.addFile(QStringLiteral(":/absence_journal/icons/excel16.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionCreateXLS = new QAction(this);
  actionCreateXLS->setObjectName(QStringLiteral("actionCreateXLS"));
  actionCreateXLS->setIcon(icon);
  actionCreateXLS->setText("Выгрузить в Excel");
  actionCreateXLS->setToolTip("Выгрузить в Excel");
  actionCreateXLS->setEnabled(true);  
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/absence_journal.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionAbsenceJournal = new QAction(this);
  actionAbsenceJournal->setObjectName(QStringLiteral("actionAbsenceJournal"));
  actionAbsenceJournal->setIcon(icon);
  actionAbsenceJournal->setText("Журнал отсутствия");
  actionAbsenceJournal->setToolTip("Журнал отсутствия на работе"); 
  
  icon.addFile(QStringLiteral(":/tree/icons/cologne-icons/ico/future-projects.ico"), QSize(), QIcon::Normal, QIcon::Off);
  actionErrandJournal = new QAction(this);
  actionErrandJournal->setObjectName(QStringLiteral("actionErrandJournal"));
  actionErrandJournal->setIcon(icon);
  actionErrandJournal->setText("Журнал местных командировок");
  actionErrandJournal->setToolTip("Журнал местных командировок"); 
  
  QMetaObject::connectSlotsByName(this);
  
  show();
  
}

SvTableViewTimeSheet::~SvTableViewTimeSheet()
{
  this->deleteLater();
}

void SvTableViewTimeSheet::contextMenuEvent(QContextMenuEvent *event)
{
  QMenu* menu = new QMenu(this);

  TreeItem* item = model->itemFromIndex(this->currentIndex());
  if(item->item_type == itEmployee)
  {
    menu->insertAction(0, actionAbsenceJournal);
    menu->insertAction(0, actionErrandJournal);
    menu->addSeparator();
  };
  
  menu->insertAction(0, actionCreateXLS);
  menu->addSeparator();
  menu->insertAction(0, actionReload);
  menu->setTitle("Табель");
  
  menu->exec(event->globalPos());
}

void SvTableViewTimeSheet::mouseDoubleClickEvent(QMouseEvent * event)
{
//  if(model->itemFromIndex(this->currentIndex())->item_type == itAbsenceJournal)
//    on_actionEditAbsenceJornal_triggered();

  TreeItem* item = model->itemFromIndex(this->currentIndex());
  
  QDate begin = QDate(year, month, 1);
  QDate end = QDate(year, month, begin.daysInMonth());
  
  switch (item->item_type)
  {
    case itDepartment:
    {
      /* создаем отчет с детализацией по дням  */
      SUBWINDOW_MAIN_UI = new SvSubWindowMain(mdiWindow, item->id, -1, begin, end, 2); // id отчета сдетализацией по дням 
      SUBWINDOW_MAIN_UI->show();
      break;
    }
       
    case itEmployee:
    {
      /* если был выбран сотрудник */
      if(item->info(this->currentIndex().column()).fieldName == "employee")
      {
        /* создаем отчет с детализацией по дням  */
        SUBWINDOW_MAIN_UI = new SvSubWindowMain(mdiWindow, -1, item->id, begin, end, 2); // id отчета сдетализацией по дням 
        SUBWINDOW_MAIN_UI->show();
        break;
      }
      
      /* если выбрано поле дня */
      else if(item->info(this->currentIndex().column()).fieldName.contains("field"))
      {
        /* в зависимости от содержимого вызывем разные журналы */
        if(item->data(this->currentIndex().column()).toString().contains("НН"))
        {
          SUBWINDOW_ABSENCE_JOURNAL_UI = new SvSubWindowAbsenceJournal(mdiWindow, -1, item->id, begin, end);
          SUBWINDOW_ABSENCE_JOURNAL_UI->show();
          break;
        }
        
        else if(item->data(this->currentIndex().column()).toString().contains("("))
        {
          SUBWINDOW_ERRAND_JOURNAL_UI = new SvSubWindowErrandJournal(mdiWindow, -1, item->id, begin, end); 
          SUBWINDOW_ERRAND_JOURNAL_UI->show();
          break;
        }
        
        else
        {
          /* создаем отчет с детализацией по дням  */
          SUBWINDOW_MAIN_UI = new SvSubWindowMain(mdiWindow, -1, item->id, begin, end, 2); // id отчета сдетализацией по дням 
          SUBWINDOW_MAIN_UI->show();
          break;
        }
      }
    }
  }
      
  QTableView::mouseDoubleClickEvent(event);
}

void SvTableViewTimeSheet::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
  TreeItem* item = model->itemFromIndex(current);
  
  actionErrandJournal->setEnabled(item->item_type == itEmployee);
  actionAbsenceJournal->setEnabled(item->item_type == itEmployee);
  
  QTableView::currentChanged(current, previous);
}

void SvTableViewTimeSheet::on_actionAbsenceJournal_triggered()
{
  TreeItem* item = model->itemFromIndex(this->currentIndex());
  
  if(item->item_type != itEmployee) return;

  QDate begin = QDate(year, month, 1);
  QDate end = QDate(year, month, begin.daysInMonth());
  
  SUBWINDOW_ABSENCE_JOURNAL_UI = new SvSubWindowAbsenceJournal(mdiWindow, -1, item->id, begin, end);
  SUBWINDOW_ABSENCE_JOURNAL_UI->show();
}

/** ********** ЖУРНАЛ МЕСТНЫХ КОМАНДИРОВОК *********** **/
void SvTableViewTimeSheet::on_actionErrandJournal_triggered()
{
  TreeItem* item = model->itemFromIndex(this->currentIndex());
  
  if(item->item_type != itEmployee) return;
  
  QDate begin = QDate(year, month, 1);
  QDate end = QDate(year, month, begin.daysInMonth());
  
  SUBWINDOW_ERRAND_JOURNAL_UI = new SvSubWindowErrandJournal(mdiWindow, -1, item->id, begin, end); 
  SUBWINDOW_ERRAND_JOURNAL_UI->show();
}

/** *************** ЗАГРУЖАЕМ ДАННЫЕ ****************** **/
void SvTableViewTimeSheet::setupModelData()
{
  showBusy("Формирование табеля. Подождите.");  
  connect(this, SIGNAL(reportCreated()), dlgBusy, SLOT(accept()));
  qApp->processEvents();

  QSqlQuery* q = new QSqlQuery(PGDB->db);
  if(QSqlError::NoError != PGDB->execSQL(QString(SQL_REPORT_TIMESHEET)
                                         .arg(this->employeeId)
                                         .arg(this->departmentId)
                                         .arg(this->month)
                                         .arg(this->year), q).type())
  {
    q->finish(); 
    delete q;
    emit reportCreated();
    return;
  }
  
  // читаем заголовки колонок
  q->first();
  
  QStringList headers = QStringList();
  for(int i = 0; i < q->record().count(); i++)
  {
    if(q->value(i).isNull()) headers << "-<";
    else headers << q->value(i).toString();
  }

  // создаем модель данных  
  this->model = new SvModelMain(headers, this);
  
  // формируем отчет
  // rootItem создается при создании объекта модели
  TreeItem *parent = model->root();
  ItemInfo inf;
  
  int child_count = 0;
  int column_count = parent->columnCount();
  while(q->next())
  {
    child_count = parent->childCount();
 
    parent->insertChildren(child_count, 1, column_count);

    parent->child(child_count)->id = q->value("id").toInt();
    
    if(q->value("duty").toString() == "department") parent->child(child_count)->item_type = itDepartment;
    else if(q->value("duty").toString() == "employee") parent->child(child_count)->item_type = itEmployee;
    
    parent->child(child_count)->bgnd_color = q->value("bgnd_color").toInt();
    parent->child(child_count)->font_color = q->value("font_color").toInt();
    parent->child(child_count)->font_bold = q->value("font_bold").toInt();
    parent->child(child_count)->font_italic = q->value("font_italic").toInt();
    
    for(int i = 0; i < q->record().count(); i ++)
    {
      inf.fieldName = q->record().fieldName(i);
      QString s = q->value(inf.fieldName).toString();
      
      int pos1 = s.indexOf("{style:");
      int pos2 = s.indexOf('}');
      if((pos1 != -1) && (pos2 > pos1))
      {
        QString style = s.mid(pos1, pos2 - pos1 + 1);
        parent->child(child_count)->setStyle(i, style);
        s.remove(style);
      }
      
      parent->child(child_count)->setData(i, s); // q->value("employee_short_name").toString());
      parent->child(child_count)->setInfo(i, inf);
      
    }
  }

  q->finish();
  free(q); 
  
  this->setModel(model);
  
  for(int i = 0; i < headers.count(); i++)
  {
    this->setColumnHidden(i, headers.at(i).startsWith('-'));
    this->resizeColumnToContents(i);
  }
//  qDebug() << headers.count();
  emit reportCreated();
}

void SvTableViewTimeSheet::showBusy(QString text)
{
  dlgBusy = new QDialog(this, Qt::Window | Qt::FramelessWindowHint);
  dlgBusy->resize(300, 100);
  dlgBusy->setStyleSheet(QStringLiteral("background-color: rgb(90, 170, 255);"));
  connect(dlgBusy, SIGNAL(accepted()), dlgBusy, SLOT(deleteLater()));
  
  QVBoxLayout* vlay = new QVBoxLayout(dlgBusy);
  
  QGroupBox* gbox = new QGroupBox(dlgBusy);
  gbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  gbox->setTitle("");
  
  QVBoxLayout* vlay2 = new QVBoxLayout(gbox);
  
  QLabel* lbl = new QLabel(gbox);
  lbl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  lbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  lbl->setText(text);
  
  vlay->addWidget(gbox);
  vlay2->addWidget(lbl);
  
  dlgBusy->setModal(true);
  dlgBusy->show();
}

void SvTableViewTimeSheet::on_actionReload_triggered()
{
  model->clear();
  this->setupModelData();
}

/** выгрузка в excel **/
void SvTableViewTimeSheet::on_actionCreateXLS_triggered()
{
  if(!model->root()->childCount()) return;

  showBusy("Формирование табеля. Подождите.");  
  connect(this, SIGNAL(reportCreated()), dlgBusy, SLOT(accept()));
  qApp->processEvents();
  
  // формируем список элементов дерева
  QList<TreeItem*> items = QList<TreeItem*>();
  QList<TreeItem*> temp = QList<TreeItem*>();
  TreeItem* parent;
  
  int counter = 0;
  int main_row_counter = 0;
  QString numeric;
  QMap<TreeItem*, QString> imap;
  
  items << model->root();
  temp  << model->root();
  while(temp.count())
  {
    parent = temp.first();
    counter = 0;
    for(int i = 0; i < parent->childCount(); i++)
    {
      temp  << parent->child(i);
      items << parent->child(i);
    }
    temp.pop_front();
  }

  // теперь необходимо расставить элементы дерева в нужном порядке
  while(items.count())
  {
    parent = items.first();
    int parent_index = temp.indexOf(parent) + 1;
    counter = 0;
    for(int i = 0; i < items.count(); i++)
    {
      
      if(items.at(i)->parent() == parent)
      {
        temp.insert(parent_index, items.at(i));
        parent_index++;
        
        if(items.at(i)->parent() == model->root())
        {
          main_row_counter++;
          
          numeric = QString::number(main_row_counter);
        }
        else
        {
          counter++;
          numeric = imap.value(parent) + "." + QString::number(counter);
        }
        imap.insert(items.at(i), numeric);
        
      }
    }
    items.pop_front();
  }

//    ////////////////////////////////////////////////////////

  EXCEL *excl = new EXCEL;

  /////////////////////////////////////////////////////////

  QList <QVariant> shap = QList <QVariant>();

  int col = 1, zdv = 1;

  /* рисуем шапку */
  shap << "№";
  for(int i = 0; i < model->root()->columnCount(); i++)
  {
    if(!this->isColumnHidden(i))
    {
      shap << model->root()->data(i);
      col++;
    }
  }
  excl->EXCEL_Range(shap, 1, 1, 1, col, true, false, 0, 0xFFFFFF, EXCEL::Border_xlMedium);

  /* выкладываем данные */
  for(int row = 0; row < temp.count(); row++)
  {
    shap.clear();
    shap << row; // imap.value(items.at(row));
//    qDebug() << "row="<<row;
    for(int column = 0; column < temp.at(row)->columnCount(); column++)
    {
      if(!this->isColumnHidden(column))
      {
        if(temp.at(row)->data(column).isNull()) shap <<  QVariant("");
        else shap << temp.at(row)->data(column);
      }
      
    }
    excl->EXCEL_Range(shap,
                      row + 1 + zdv, 1, row + 1 + zdv, col,
                      temp.at(row)->font_bold, temp.at(row)->font_italic,
                      temp.at(row)->font_color, temp.at(row)->bgnd_color,
                      EXCEL::Border_xlThin);
    
  }

  excl->AutoFit(1, 100);
  excl->Visible();
  
  emit this->reportCreated();

}
