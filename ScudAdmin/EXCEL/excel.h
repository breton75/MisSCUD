#ifndef EXCEL_H
#define EXCEL_H

#include <QAxWidget>
#include <QAxObject>
#include <QtEndian>

//struct alignment_st {


//    int Top= -4160 ;
//    int Bottom= -4107;
//    int Left= -4131;
//    int Right= -4152;
//    int Center= -4108;

//};
class EXCEL
{
public:
enum alignment{
     alignment_xlTop= -4160 ,
     alignment_xlBottom= -4107,
     alignment_xlCenter= -4108,
    ////HorizontalAlignment
    alignment_xlLeft= -4131,
    alignment_xlRight= -4152,
    ///VerticalAlignment
    alignment_xlDistributed=-4117,
    alignment_xlJustify=-4130

};

enum Border{
    Border_xlHairline=    1,
    Border_xlMedium=    -4138,
    Border_xlThick=    4,
    Border_xlThin=    2

};
EXCEL();
    ~EXCEL();
   //alignment_st alignment;
   // на директорию, откуда грузить книгу
    EXCEL_Open(QString Path);
    // вставка значения переменной data (любой тип, приводимый к QVariant) в полученную ячейку
    EXCEL_cell(QVariant data, QVariant row,QVariant col,  bool Bold,bool Border, int TBorder);
    //Вставка данных в прямоугольную область
    EXCEL_Range(QList<QVariant> data, QVariant row_top,QVariant col_top, QVariant row_low,QVariant col_low);
    EXCEL_Range(QList<QVariant> data, QVariant row_top,QVariant col_top, QVariant row_low,QVariant col_low, Border Border);
    EXCEL_Range(QList<QVariant> data, QVariant row_top,QVariant col_top, QVariant row_low,QVariant col_low, bool Bold);
    EXCEL_Range(QList<QVariant> data, QVariant row_top,QVariant col_top, QVariant row_low,QVariant col_low, bool Bold, Border Border);
    EXCEL_Range(QList<QVariant> data, QVariant row_top,QVariant col_top, QVariant row_low,QVariant col_low, alignment alignment);
    EXCEL_Range(QList<QVariant> data, QVariant row_top,QVariant col_top, QVariant row_low,QVariant col_low, Border Border, alignment alignment);
    EXCEL_Range(QList<QVariant> data, QVariant row_top,QVariant col_top, QVariant row_low,QVariant col_low, bool Bold, alignment alignment);
    EXCEL_Range(QList<QVariant> data, QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, bool Bold, bool Italic, int FontColor, int BgndColor, Border Border, alignment alignment = alignment_xlLeft);
    // получение указателя на ячейку [row][col] ((!)нумерация с единицы)
    QVariant EXCEL_cell_read( QVariant row,QVariant col);
    QVariant EXCEL_cell_Font(QAxObject *Cells,  bool Bold, bool Italic, int Color);
    void Range_Borders(QVariant row_top, QVariant col_top, QVariant row_low, QVariant col_low, int t);
    void AutoFit(QVariant row, QVariant col);
    void Borders(QAxObject *range, int t);
    void Visible();
    void Save(QString path);
private:
    // переменные
    QAxObject *mExcel;

    // на директорию, откуда грузить книгу1
    QAxObject *workbook;
    QAxObject *workbooks;
    // на листы (снизу вкладки)
    QAxObject *mSheets;

    QAxObject * worksheet;
};



#endif // EXCEL_H
/*
 * xlAll	-4104
xlAutomatic	-4105
xlBoth	1
xlCenter	-4108
xlChecker	9
xlCircle	8
xlCorner	2
xlCrissCross	16
xlCross	4
xlDiamond	2
xlDistributed	-4117
xlDoubleAccounting	5
xlFixedValue	1
xlFormats	-4122
xlGray16	17
xlGray8	18
xlGrid	15
xlHigh	-4127
xlInside	2
xlJustify	-4130
xlLightDown	13
xlLightHorizontal	11
xlLightUp	14
xlLightVertical	12
xlLow	-4134
xlManual	-4135
xlMinusValues	3
xlModule	-4141
xlNextToAxis	4
xlNone	-4142
xlNotes	-4144
xlOff	-4146
xlOn	1
xlPercent	2
xlPlus	9
xlPlusValues	2
xlSemiGray75	10
xlShowLabel	4
xlShowLabelAndPercent	5
xlShowPercent	3
xlShowValue	2
xlSimple	-4154
xlSingle	2
xlSingleAccounting	4
xlSolid	1
xlSquare	1
xlStar	5
xlStError	4
xlToolbarButton	2
xlTriangle	3
xlGray25	-4124
xlGray50	-4125
xlGray75	-4126
xlBottom	-4107
xlLeft	-4131
xlRight	-4152
xlTop	-4160
xl3DBar	-4099
xl3DSurface	-4103
xlBar	2
xlColumn	3
xlCombination	-4111
xlCustom	-4114
xlDefaultAutoFormat	-1
xlMaximum	2
xlMinimum	4
xlOpaque	3
xlTransparent	2
xlBidi	-5000
xlLatin	-5001
xlContext	-5002
xlLTR	-5003
xlRTL	-5004
xlFullScript	1
xlPartialScript	2
xlMixedScript	3
xlMixedAuthorizedScript	4
xlVisualCursor	2
xlLogicalCursor	1
xlSystem	1
xlPartial	3
xlHindiNumerals	3
xlBidiCalendar	3
xlGregorian	2
xlComplete	4
xlScale	3
xlClosed	3
xlColor1	7
xlColor2	8
xlColor3	9
xlConstants	2
xlContents	2
xlBelow	1
xlCascade	7
xlCenterAcrossSelection	7
xlChart4	2
xlChartSeries	17
xlChartShort	6
xlChartTitles	18
xlClassic1	1
xlClassic2	2
xlClassic3	3
xl3DEffects1	13
xl3DEffects2	14
xlAbove	0
xlAccounting1	4
xlAccounting2	5
xlAccounting3	6
xlAccounting4	17
xlAdd	2
xlDebugCodePane	13
xlDesktop	9
xlDirect	1
xlDivide	5
xlDoubleClosed	5
xlDoubleOpen	4
xlDoubleQuote	1
xlEntireChart	20
xlExcelMenus	1
xlExtended	3
xlFill	5
xlFirst	0
xlFloating	5
xlFormula	5
xlGeneral	1
xlGridline	22
xlIcons	1
xlImmediatePane	12
xlInteger	2
xlLast	1
xlLastCell	11
xlList1	10
xlList2	11
xlList3	12
xlLocalFormat1	15
xlLocalFormat2	16
xlLong	3
xlLotusHelp	2
xlMacrosheetCell	7
xlMixed	2
xlMultiply	4
xlNarrow	1
xlNoDocuments	3
xlOpen	2
xlOutside	3
xlReference	4
xlSemiautomatic	2
xlShort	1
xlSingleQuote	2
xlStrict	2
xlSubtract	3
xlTextBox	16
xlTiled	1
xlTitleBar	8
xlToolbar	1
xlVisible	12
xlWatchPane	11
xlWide	3
xlWorkbookTab	6
xlWorksheet4	1
xlWorksheetCell	3
xlWorksheetShort	5
xlAllExceptBorders	7
xlLeftToRight	2
xlTopToBottom	1
xlVeryHidden	2
xlDrawingObject	14

*/
