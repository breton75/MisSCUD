#include "sv_model_main.h"

extern SvPGDB* PGDB;

SvModelMain::SvModelMain(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
  foreach (QString header, headers)
  {
    // выравнивание в ячейке
    switch (header.at(1).unicode())
    {
      case char('|'):  // '|'
        alignments << Qt::AlignHCenter; break;
        
      case char('>'): // '>'
        alignments << Qt::AlignRight; break;
        
      default:
        alignments << Qt::AlignLeft; break;
    }

    rootData << header.remove(0, 2);
  }

  rootItem = new TreeItem(rootData);

}

SvModelMain::~SvModelMain()
{
    delete rootItem;
}

int SvModelMain::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}

QVariant SvModelMain::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    
    TreeItem *item = getItem(index);
    ItemInfo info = item->info(index.column());
//    if(!item) return QVariant();
    
    switch(role)
    {
      case Qt::DisplayRole:  
      case Qt::EditRole:
        return item->data(index.column());
          
      case Qt::FontRole:
      {
        QFont fnt = QFont();
        fnt.setBold(item->style(index.column()).font_bold.isValid()
                    ? item->style(index.column()).font_bold.toBool()
                    : item->font_bold);
        
        fnt.setItalic(item->style(index.column()).font_italic.isValid()
                      ? item->style(index.column()).font_italic.toBool()
                      : item->font_italic);
        return fnt;
      }
        
      case Qt::TextAlignmentRole:
        return alignments.at(index.column()) | Qt::AlignVCenter;
        
      case Qt::ForegroundRole:
      {
        int cl = (!item->style(index.column()).fgnd_color.isValid() 
                  ? item->font_color 
                  : item->style(index.column()).fgnd_color.toInt());
        
        return QBrush(QColor(cl));
      }
        
      case Qt::BackgroundRole:
      {
        int cl = (!item->style(index.column()).bgnd_color.isValid() 
                  ? item->bgnd_color 
                  : item->style(index.column()).bgnd_color.toInt());
        
        return QBrush(QColor(cl));
      }

//        break;  
        /* парадокс! если раскомментировать break, то release сборка начинает 
         * вываливаться с ощибкой при работе с моделью */
        
      case Qt::DecorationRole:
        if(info.type == itScudDeviceName)
        {
          switch (item->item_state)
          {
            case 1:
              return QIcon(":/reports/icons/cologne-icons/ico/income.ico");
              
            case 2:
              return QIcon(":/reports/icons/cologne-icons/ico/outcome.ico");

              
          }
        }
//        break;
        
      default:
        return QVariant();
          
    }
}

Qt::ItemFlags SvModelMain::flags(const QModelIndex &index) const
{
  if (!index.isValid()) return 0;
  
  Qt::ItemFlags flags =  QAbstractItemModel::flags(index);
  TreeItem* item = getItem(index);
  
  if(!editable | item->is_main_row) return flags ;
  
  switch (item->info(index.column()).type)
  {
    case itEmployeeLoadAllot:  
      flags |= Qt::ItemIsEditable;

  }
  
  return flags;
}

TreeItem *SvModelMain::getItem(const QModelIndex &index) const
{
  if (index.isValid()) {
      TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
      if (item)
          return item;
  }
  return rootItem;
}

TreeItem *SvModelMain::itemFromIndex(const QModelIndex &index) const
{
  if(index.isValid())
  {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (item) return item;
  }
  
  return rootItem;
}

QVariant SvModelMain::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
  if(orientation != Qt::Horizontal /*&& role == Qt::DisplayRole*/) return QVariant();
  
  switch (role)
  {
    case Qt::DisplayRole:
      return rootItem->data(section);
//      break;
      
    case Qt::FontRole:
    {
      QFont boldFont = QFont();
      boldFont.setBold(true);
      return boldFont;
//      break; 
    }
      
    case Qt::TextAlignmentRole:
      return Qt::AlignCenter;
      
    default:
      return QVariant();
  }
}

QModelIndex SvModelMain::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool SvModelMain::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success = false;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool SvModelMain::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = false;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex SvModelMain::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool SvModelMain::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success = false;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool SvModelMain::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

void SvModelMain::clear()
{
  if(rootItem->childCount())
  {
    QList<TreeItem*> items = QList<TreeItem*>();
    QList<TreeItem*> del = QList<TreeItem*>();

    items << rootItem;
    del   << rootItem;
    while(items.count())
    {
      TreeItem* parent = items.first();
      
      for(int i = 0; i < parent->childCount(); i++)
      {
        del   << parent->child(i);
        items << parent->child(i);
      }
      
      items.pop_front();
      
    }

    // удаляем начиная с конца, т.е. сначала удаляем наследников, потом, 
    // поднимаясь наверх - предков, пока не дойдем до rootItem
    beginResetModel();

    while(1)
    {
//      qDebug() << del.last()->data(0).toString();
      del.last()->removeChildren(0, del.last()->childCount());
      if(rootItem == del.last()) break;
      del.pop_back();
    }
//    rootItem->removeChildren(0, 3/*rootItem->childCount()*/); //! !!!!!!!!!
    endResetModel();
    
  }
}

int SvModelMain::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool SvModelMain::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole) return false;

    TreeItem *item = getItem(index);

    QString sql = "";
    switch (item->info(index.column()).type)
    {
//      case itTaskName:
//        sql = QString("update tasks set task_name = '%1' where task_name = '%2'")
//            .arg(value.toString())
//            .arg(item->data(index.column()).toString());
//        break;
        
//      case itTaskBegin:
//        sql = QString("update tasks set task_begin = '%1' where task_name = '%2'")
//            .arg(value.toString())
//            .arg(item->data(index.column()).toString());
//        break;
        
//      case itTaskLaboriousness:
//        sql = QString("update tasks set laboriousness = %1 where id = %2")
//            .arg(value.toFloat())
//            .arg(item->id);
//        break;           
        
      case itEmployeeLoadAllot:
        sql = QString("update general set %1 = %2 where task_id = %3")
            .arg(item->info(index.column()).fieldName)
            .arg(value.toFloat())
            .arg(item->id);
        break;
        
      default:
        return false;
    }
    
    // сохраняем новое значение в базу
    QSqlError err = PGDB->execSQL(sql);
    if(err.type() != QSqlError::NoError) return false;
    
//    // если изменилось значение трудоемкости, то высчитываем сумму трудоемкости для задачи
//    if(item->info(index.column()).type == itEmployeeLoadPlan)
//    {
//      // ищем номер столбца суммы трудоемкости для задачи
//      int i = 0;
//      while(item->info(i).type != itTaskSum) i++;
      
//      QSqlQuery* q = new QSqlQuery(PGDB->db);
//      QSqlError err = PGDB->execSQL(QString("select task_sum from get_task_sum(%1)")
//                                    .arg(item->id), q);
//      if(err.type() != QSqlError::NoError) return false;
//      q->first();
      
//      // заносим полученное значение в столбец суммы
//      item->setData(i, q->value("task_sum").toFloat());
      
//      q->finish();
//      free(q);
      
//    }

    bool result = item->setData(index.column(), value);
    
    if (result) emit dataChanged(index, index);
    
    return result;
}

bool SvModelMain::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

