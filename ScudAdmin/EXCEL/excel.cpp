#include "excel.h"
#include <QDebug>
EXCEL::EXCEL()
{

        // получаем указатель на Excel
       mExcel = new QAxObject( "Excel.Application");
        // на книги
    workbooks = mExcel->querySubObject("WorkBooks");
               workbooks->dynamicCall("Add");
       workbook = mExcel->querySubObject( "ActiveWorkBook" );


                // на листы (снизу вкладки)
      //  mSheets = workbook->querySubObject( "WorkSheets" );
       mSheets = workbook->querySubObject("Worksheets(int)", 1);

}


EXCEL::~EXCEL()
{

    delete workbooks;
    delete mSheets;
    delete workbook;
    delete mExcel;
}


EXCEL::EXCEL_Open(QString Path)
{
    // на директорию, откуда грузить книгу
    QAxObject *workbooks = mSheets->querySubObject( "Open(const QString&)", Path );
}

EXCEL::EXCEL_cell(QVariant data,QVariant row, QVariant col,  bool Bold, bool Border, int TBorder)
{
    // получение указателя на ячейку [row][col] ((!)нумерация с единицы)
    QAxObject* cell = mSheets->querySubObject("Cells (QVariant&,QVariant&)",row ,col);

    // QAxObject* cell1 = mSheets->querySubObject("Range (QVariant&,QVariant&)",row ,col);
    cell->setProperty("NumberFormat","@");
    // вставка значения переменной data (любой тип, приводимый к QVariant) в полученную ячейку

     cell->setProperty("Value", QVariant(data));

    // освобождение памяти
    EXCEL_cell_Font(cell,  Bold, false, 0);
    if (Border)Borders(cell,TBorder);
    delete cell;

}

EXCEL::EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low)
{
    if (col_low<=1)col_low=1;
if (row_low<=1)row_low=1;
    QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
    // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
    QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
    // получение указателя на целевую область
    QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
    // список ячеек строки
    // собственно вывод
    range->setProperty("NumberFormat","@");
    range->setProperty("Value",data );

    // освобождение памяти
    delete range;
    delete Cell1;
    delete Cell2;
}

EXCEL::EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, Border Border)
{
try
{
    if (col_low<=1)col_low=1;
if (row_low<=1)row_low=1;
    QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
    // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
    QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
    // получение указателя на целевую область
    QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
    // список ячеек строки
    // собственно вывод
    range->setProperty("NumberFormat","@");
    range->setProperty("Value",data );
//    range->dynamicCall("Select");
    //qDebug()<<"1";
   // QAxObject* Select=mSheets->querySubObject("Selection" );
//qDebug()<<"2";
        //Range_Borders(row_top,col_top,row_low,col_low ,TBorder);
        Borders(range,Border);

    //qDebug()<<"3";
    // освобождение памяти
    delete range;
    delete Cell1;
    delete Cell2;
}
catch (const std::bad_alloc &)
    {
           qDebug()<<"error "<<row_low<<"  "<<col_low;
    }
}

EXCEL::EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, bool Bold)
{
    if (col_low<=1)col_low=1;
if (row_low<=1)row_low=1;
    QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
    // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
    QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
    // получение указателя на целевую область
    QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
    // список ячеек строки
    // собственно вывод
    range->setProperty("NumberFormat","@");
    range->setProperty("Value",data );
    
EXCEL_cell_Font(range,  Bold, false, 0);
    // освобождение памяти
    delete range;
    delete Cell1;
delete Cell2;
}

EXCEL::EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, bool Bold, Border Border)
{
    try
    {
        if (col_low<=1)col_low=1;
    if (row_low<=1)row_low=1;
        QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
        // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
        QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
        // получение указателя на целевую область
        QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
        // список ячеек строки
        // собственно вывод
        range->setProperty("NumberFormat","@");
        range->setProperty("Value",data );
       // range->dynamicCall("Select");
        //qDebug()<<"1";
       // QAxObject* Select=mSheets->querySubObject("Selection" );
    //qDebug()<<"2";
            //Range_Borders(row_top,col_top,row_low,col_low ,TBorder);
            Borders(range,Border);
    EXCEL_cell_Font(range,  Bold, false, 0);


        //qDebug()<<"3";
        // освобождение памяти
        delete range;
        delete Cell1;
        delete Cell2;
    }
    catch (const std::bad_alloc &)
        {
               qDebug()<<"error "<<row_low<<"  "<<col_low;
    }
}

EXCEL::EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, alignment alignment)
{

        if (col_low<=1)col_low=1;
    if (row_low<=1)row_low=1;
        QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
        // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
        QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
        // получение указателя на целевую область
        QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
        // список ячеек строки
        // собственно вывод
        range->setProperty("NumberFormat","@");
        range->setProperty("Value",data );

            range->setProperty("HorizontalAlignment",alignment  );

    range->setProperty("VerticalAlignment",alignment  );

        delete range;
        delete Cell1;
    delete Cell2;
}

EXCEL::EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, Border Border, EXCEL::alignment alignment)
{
    if (col_low<=1)col_low=1;
if (row_low<=1)row_low=1;
    QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
    // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
    QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
    // получение указателя на целевую область
    QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
    // список ячеек строки
    // собственно вывод
    range->setProperty("NumberFormat","@");
    range->setProperty("Value",data );
if (alignment==alignment_xlJustify | alignment==alignment_xlDistributed )range->setProperty("HorizontalAlignment",alignment_xlCenter  );
else                                                 range->setProperty("HorizontalAlignment",alignment  );


if (alignment==alignment_xlLeft | alignment==alignment_xlRight )         range->setProperty("VerticalAlignment",alignment_xlCenter  );
else                                                 range->setProperty("VerticalAlignment",alignment  );
    Borders(range,Border);
    delete range;
    delete Cell1;
    delete Cell2;
}

EXCEL::EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, bool Bold, EXCEL::alignment alignment)
{
    if (col_low<=1)col_low=1;
if (row_low<=1)row_low=1;
    QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
    // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
    QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
    // получение указателя на целевую область
    QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
    // список ячеек строки
    // собственно вывод
    range->setProperty("NumberFormat","@");
    range->setProperty("Value",data );

    if (alignment==alignment_xlJustify | alignment==alignment_xlDistributed )range->setProperty("HorizontalAlignment",alignment_xlCenter  );
    else                                                 range->setProperty("HorizontalAlignment",alignment  );


    if (alignment==alignment_xlLeft | alignment==alignment_xlRight )         range->setProperty("VerticalAlignment",alignment_xlCenter  );
    else                                                 range->setProperty("VerticalAlignment",alignment  );
    //Borders(range,Border);
     EXCEL_cell_Font(range,  Bold, false, 0);
    delete range;
    delete Cell1;
     delete Cell2;
}

EXCEL::EXCEL_Range(QList<QVariant> data,
                   QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low,
                   bool Bold, bool Italic, int FontColor, int BgndColor,
                   Border Border, EXCEL::alignment alignment)
{
  if (col_low <= 1)col_low=1;
  if (row_low <= 1)row_low=1;
  
  QAxObject* Cell1 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_top, col_top);
  // получение указателя на правую нижнюю ячейку [row + numRows - 1][col + numCols - 1] ((!) numRows>=1,numCols>=1)
  QAxObject* Cell2 = mSheets->querySubObject("Cells (QVariant&,QVariant&)", row_low, col_low);
  // получение указателя на целевую область
  QAxObject* range = mSheets->querySubObject("Range(const QVariant&,const QVariant&)", Cell1->asVariant(), Cell2->asVariant() );
  // список ячеек строки
  // собственно вывод
  range->setProperty("NumberFormat","@");
  range->setProperty("Value",data );
//  range->dynamicCall("SetColor", FontColor);
  
  /* цвет фона */
  QAxObject* interior = range->querySubObject("Interior");
  interior->setProperty("Color", qToBigEndian(BgndColor) >> 8);
  
  if (alignment == alignment_xlJustify | alignment == alignment_xlDistributed )
    range->setProperty("HorizontalAlignment", alignment_xlCenter);
  else range->setProperty("HorizontalAlignment", alignment);

  if (alignment==alignment_xlLeft | alignment==alignment_xlRight )         range->setProperty("VerticalAlignment",alignment_xlCenter  );
  else range->setProperty("VerticalAlignment",alignment  );

  Borders(range, Border);
  EXCEL_cell_Font(range, Bold, Italic, FontColor);
  
  
  free(interior);
  free(range);
  free(Cell1);
  free(Cell2);
}


void EXCEL::Visible()
{
    mExcel->setProperty("Visible", true);
}

void EXCEL::Save(QString path)
{
  workbook->dynamicCall("SaveAs (const QString&)", path);
}


void EXCEL::Borders(QAxObject *range, int t)
{
    QAxObject *border = range->querySubObject("Borders(xlEdgeTop)");

    //  xlEdgeTop(верхняя граница) (xlEdgeLeft) левая, (xlEdgeRight) правая,(xlEdgeBottom) нижняя и
    //2 диагонали (xlDiagonalDown) (xlDiagonalUp)
       border->setProperty("LineStyle",1); //тип линии (там пунктиры,сплошная и так далее)

       border->setProperty("Weight",t);  //толщина

       border = range->querySubObject("Borders(xlEdgeTop)");
           //  xlEdgeTop(верхняя граница) (xlEdgeLeft) левая, (xlEdgeRight) правая,(xlEdgeBottom) нижняя и
           //2 диагонали (xlDiagonalDown) (xlDiagonalUp)
              border->setProperty("LineStyle",1); //тип линии (там пунктиры,сплошная и так далее)
              border->setProperty("Weight",t);  //толщина

              border = range->querySubObject("Borders(xlEdgeLeft)");
                  //  xlEdgeTop(верхняя граница) (xlEdgeLeft) левая, (xlEdgeRight) правая,(xlEdgeBottom) нижняя и
                  //2 диагонали (xlDiagonalDown) (xlDiagonalUp)
                     border->setProperty("LineStyle",1); //тип линии (там пунктиры,сплошная и так далее)
                     border->setProperty("Weight",t);  //толщина



                     border = range->querySubObject("Borders(xlEdgeRight)");
                         //  xlEdgeTop(верхняя граница) (xlEdgeLeft) левая, (xlEdgeRight) правая,(xlEdgeBottom) нижняя и
                         //2 диагонали (xlDiagonalDown) (xlDiagonalUp)
                            border->setProperty("LineStyle",1); //тип линии (там пунктиры,сплошная и так далее)
                            border->setProperty("Weight",t);  //толщина

                            border = range->querySubObject("Borders(xlEdgeBottom)");
                                //  xlEdgeTop(верхняя граница) (xlEdgeLeft) левая, (xlEdgeRight) правая,(xlEdgeBottom) нижняя и
                                //2 диагонали (xlDiagonalDown) (xlDiagonalUp)
                                   border->setProperty("LineStyle",1); //тип линии (там пунктиры,сплошная и так далее)
                                   border->setProperty("Weight",t);  //толщина


                                   border = range->querySubObject("Borders(xlInsideVertical)");
                                       //  xlEdgeTop(верхняя граница) (xlEdgeLeft) левая, (xlEdgeRight) правая,(xlEdgeBottom) нижняя и
                                       //2 диагонали (xlDiagonalDown) (xlDiagonalUp)
                                          border->setProperty("LineStyle",1); //тип линии (там пунктиры,сплошная и так далее)
                                          border->setProperty("Weight",t);  //толщина


                                          border = range->querySubObject("Borders(xlInsideHorizontal)");
                                              //  xlEdgeTop(верхняя граница) (xlEdgeLeft) левая, (xlEdgeRight) правая,(xlEdgeBottom) нижняя и
                                              //2 диагонали (xlDiagonalDown) (xlDiagonalUp)
                                                 border->setProperty("LineStyle",1); //тип линии (там пунктиры,сплошная и так далее)
                                                 border->setProperty("Weight",t);  //толщина

}




void EXCEL::AutoFit(QVariant row, QVariant col)
{
    QString ddd=QString("%1:%2").arg(row.toString()).arg(col.toString());

QAxObject *rangec = mSheets->querySubObject( "Range(const QVariant&)",QVariant(ddd));

QAxObject *Columns = rangec->querySubObject("Columns");

//QAxObject *AutoFit = Columns->querySubObject("AutoFit");
Columns->dynamicCall("AutoFit");

//delete AutoFit;

delete Columns;

delete rangec;


}

QVariant EXCEL::EXCEL_cell_read(QVariant row, QVariant col)
{
    // получение указателя на ячейку [row][col] ((!)нумерация с единицы)
    QAxObject* cell = mSheets->querySubObject("Cells(QVariant,QVariant)", row, col);
    // получение содержимого
   return  cell->property("Value");
    // освобождение памяти
delete cell;
}

QVariant EXCEL::EXCEL_cell_Font(QAxObject *Cells,  bool Bold, bool Italic, int Color)
{
     //QAxObject* cell = mSheets->querySubObject("Cells(QVariant,QVariant)", row, col);
     QAxObject *pFont = Cells->querySubObject("Font"); //получаем шрифт в выбранной ячейке
     //pFont->setProperty("Size", 15); //задаем размер шрифта
     pFont->setProperty("Bold", Bold); //делаем его полужирным ...
     pFont->setProperty("Italic", Italic);

    /* цвет нужно перевернуть */
     pFont->setProperty("Color", qToBigEndian(Color) >> 8);

    free(pFont);
     //delete cell;
}



