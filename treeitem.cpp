/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include "treeitem.h"

#include <QStringList>

//! [0]
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
    
    ItemInfo inf = ItemInfo();
    ItemStyle style = ItemStyle();
    for(int i = 0; i < itemData.count(); i++)
    {
      itemInfo.append(inf);
      itemStyle.append(style);
    }
    
}
//! [0]

//! [1]
TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}
//! [1]

//! [2]
TreeItem *TreeItem::child(int number)
{
    return childItems.value(number);
}
//! [2]

//! [3]
int TreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
    return itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}

ItemInfo TreeItem::info(int column) const
{
    return itemInfo.value(column);
}

ItemStyle TreeItem::style(int column) const
{
    return itemStyle.value(column);
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}
//! [7]

//! [8]
bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
    {
        itemData.insert(position, QVariant());
        ItemInfo inf;
        itemInfo.insert(position, inf);
        ItemStyle style;
        itemStyle.insert(position, style);
    }

    foreach (TreeItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}
//! [8]

//! [9]
TreeItem *TreeItem::parent()
{
    return parentItem;
}
//! [9]

//! [10]
bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
//! [10]

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
    {
        itemData.remove(position);
        itemInfo.remove(position);
        itemStyle.remove(position);
    }

    foreach (TreeItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}

bool TreeItem::setInfo(int column, ItemInfo info)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemInfo[column] = info;

    return true;
}

bool TreeItem::setStyle(int column, ItemStyle style)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemStyle[column] = style;

    return true;
}

bool TreeItem::setStyle(int column, QString style)
{
    if (column < 0 || column >= itemData.size())
        return false;
    
    if(!style.toLower().contains("{style:")) return false;
    
    style.remove("{style:");
    style.remove("}");
    
    QStringList lst = style.split(';');
    for(int i = 0; i < lst.count(); i++)
    {
      if(lst.at(i).contains("fgnd_color:"))
      {
        QString scl = lst.at(i);
        scl = scl.remove("fgnd_color:").trimmed();
        bool ok;
        int cl = scl.toInt(&ok, 16);
        itemStyle[column].fgnd_color = (ok ? cl : NULL);
      }
      else if(lst.at(i).contains("bgnd_color:"))
      {
        QString scl = lst.at(i);
        scl = scl.remove("bgnd_color:").trimmed();
        bool ok;
        int cl = scl.toInt(&ok, 16);
        itemStyle[column].bgnd_color = (ok ? cl : NULL);
      }
      else if(lst.at(i).contains("font_bold:"))
      {
        QString scl = lst.at(i);
        scl = scl.remove("font_bold:").trimmed();
        itemStyle[column].font_bold = bool(scl.toLower() == "true");
      }
      else if(lst.at(i).contains("font_italic:"))
      {
        QString scl = lst.at(i);
        scl = scl.remove("font_italic:").trimmed();
        itemStyle[column].font_italic = bool(scl.toLower() == ("true"));
      }
    }

    return true;
}
