#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QSqlQueryModel>

#include "sv_treeview_misscud.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    SvTreeView* tree;
    QTextEdit* get_log();
    
    void AfterShow();
    
  private:
    Ui::MainWindow *ui;
    
  private slots:
    void duty();

};

#endif // MAINWINDOW_H
