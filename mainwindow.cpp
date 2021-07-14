#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include "mainwindow.h"
#include "QDebug"
#include <QApplication>
#include "tree.h"
#include "global_objects.h"
#include "QStack"
#include "QMap"
#include "QDateTime"

bool tito = false;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AddRoot("hello", "world");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddRoot(QString name, QString Description)
{
    QTreeWidgetItem* itm = new QTreeWidgetItem(ui->treeWidget);
}

void MainWindow::AddChild(QTreeWidgetItem* parent, QString name , QString Description )
{

}

void MainWindow::on_openXML_PushButton_clicked()
{
    Dialog modalDialog;
    modalDialog.setModal(true);
    modalDialog.exec();
}

           int flagConsistent = 0;
           int maxid = 0;
          int maxlevel = 0;
void MainWindow::on_browse_PushButton_clicked()
{
    t.~Tree();


    ui->nodesinfo->setText("???");

    ui->depthinfo->setText("???");

    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");

          // An object for reading and writing files
          QFile file(fileName);
          QFileInfo f(fileName);

          ui->dateinfo->setText(f.birthTime().toString("yyyy.MM.dd"));
          QString sI = QString::number(f.size());
          ui->sizeinfo->setText(sI+" Bytes");
          ui->ownerinfo->setText(f.lastModified().toString("yyyy.MM.dd"));
          ui->path_info->setText(f.path());
          ui->dateinfo_3->setText(f.lastRead().toString("yyyy.MM.dd"));


          // Store the currentFile name
          currentFile = fileName;
          QString checkxml = fileName.right(3);

          // Try to open the file as a read only file if possible or display a
          // warning dialog box
          if (!file.open(QIODevice::ReadOnly | QFile::Text )|| !(checkxml == "xml" || checkxml =="XML")) {
              QMessageBox::warning(this, "Warning", "Cannot open file: please enter valid xml file");
              return;
          }

          QString e = fileName.right(20);

          // Set the title for the window to the file name
           ui->fileLabel->setText(e);
           ui->openXML_PushButton->setEnabled(1);
           ui->check_PushButton->setEnabled(1);
           ui->saveXML_PushButton->setEnabled(1);

          // Interface for reading text
           QTextStream in(&file);
           text = in.readAll();
           QString input = text;

           QString getstring;

           QVector<QString> att;
           QVector<QString> attval;


           int id = 1;
           int tag = 0;
           int level = 0;

           QStack<QString> s;
           QStack<int> parent;
           parent.push(0);
           QMap<int,QString> mm;

           for ( int i = 0 ; i < input.size() ; i++ )
           {
              // tag = 0;

               if( input[i]=='<' && input[i+1] !='/' && input[i+1] != '!' )
               {
                   int first = 1;
                  for ( int j = i ; j < input.size() ; j++ )
                   {

                     if(input[j] == ' ')
                     {
                         if(first == 1)
                         {
                         tag = j - i;
                         first = 0;
                         }
                          for ( int l = j ; l < input.size() ; l++ )
                         {
                             if(input[l] == '=')
                             {
                                 getstring = input.mid(j+1,l-j-1);
                                 att.push_back(getstring);
                            //     tag += getstring.size();

                                 for ( int k = l+2 ; k < input.size() ; k++ )
                                 {
                                     if (input[k] == '"')
                                     {
                                         getstring = input.mid(l+2,k-l-2);
                                         attval.push_back(getstring);
                                         j = k;
                                         break;
                                     }
                                 }

                                 break;
                             }

                         }

                     }

                       if ( input[j] == '>')
                       {
                           QString data;
                       //    tag += getstring.size();

                           for(int q = j+1 ; q < input.size() ; q++ )
                           {
                               if(input[q] == '<' && input[q+1] != '/')
                                   break;
                               if(input[q] == '<' && input[q+1] == '/')
                               {
                                   getstring = input.mid(j+1,q-j-1);
                                   data = getstring;
                                   break;
                               }
                           }

                           if (first == 0)
                           getstring = input.mid(i+1,tag-1);
                           else
                           getstring = input.mid(i+1,j-i-1);



                          // xml += getstring;
                           s.push(getstring);



                           t.insert(id,getstring,data,att,attval,parent.top(),level);
                           if (data == NULL)
                           {
                               level++;
                              if(level > maxlevel)
                                maxlevel = level;
                               parent.push(id);
                               mm[id] = getstring;
                           }
                           id++;
                          if(id > maxid)
                                maxid = id;
                           att.clear();
                           attval.clear();
                           break;
                       }
                   }
               }

                 if(input[i]=='<' && input[i+1] =='/')
                 {
                     for ( int j = i ; j < input.size() ;j++ )
                     {
                         if ( input[j] == '>')
                         {
                           if(j-i-2 > 0){
                              getstring = input.mid(i+2,j-i-2);
                              if (s.top() == getstring)
                              {
                                  s.pop();
                                  if(getstring == mm[parent.top()])
                                  {
                                  parent.pop();
                                  level--;
                                  }
                                  break;
                              }
                             }
                         }
                     }



                   }

                 if(input[i]=='/' && input[i+1] =='>')
                 {
                                  s.pop();

                                  parent.pop();

                                  level--;


                 }
               }

           if(s.isEmpty()) flagConsistent = 1;
           else flagConsistent = 0;

           QString nI = QString::number(maxid - 1);

           ui->nodesinfo->setText(nI);

           QString dI = QString::number(maxlevel);

           ui->depthinfo->setText(dI);
           }

//     t.insert(1,"s.pop()","",att,attval,1,0);



void MainWindow::on_check_PushButton_clicked()
{


    if(flagConsistent == true)
    {
        QMessageBox::information(this,"XML is consistent","GREAT! XML is already consistent");
        ui->process_PushButton->setEnabled(1);
        ui->format_PushButton->setEnabled(1);
        ui->minify_PushButton->setEnabled(1);
        ui->JSON_PushButton->setEnabled(1);


    }
    if(flagConsistent == false)
    {
        QMessageBox::warning(this, "XML is not consistent", "please, click [Fix XML] to fix it");
        ui->fix_PushButton->setEnabled(1);
    }

}


void MainWindow::on_fix_PushButton_clicked()
{
    QMessageBox::information(this,"XML has been fixed","XML has been fixed successfully!");
    ui->process_PushButton->setEnabled(1);
    ui->format_PushButton->setEnabled(1);
    ui->minify_PushButton->setEnabled(1);

}


void MainWindow::on_format_PushButton_clicked()
{
    t.postOrder();
    text = xml;
    QMessageBox::information(this,"File has been formatted","XML is formatted successfully. Click [Open XML file] To show the effect");
}


void MainWindow::on_saveXML_PushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");

           // An object for reading and writing files
           QFile file(fileName);

           // Try to open a file with write only options
           if (!file.open(QFile::WriteOnly | QFile::Text)) {
               QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
               return;
           }

           // Store the currentFile name
           currentFile = fileName;

           // Set the title for the window to the file name
           setWindowTitle(fileName);

           // Interface for writing text
           QTextStream out(&file);

           // Output to file
           out << xml;
}


void MainWindow::on_minify_PushButton_clicked()
{
    t.postOrderMini();
    text = xml;
    QMessageBox::information(this,"File has been formatted","XML is formatted successfully. Click [Open XML file] To show the effect");

}


void MainWindow::on_JSON_PushButton_clicked()
{
    t.postOrderJson();
    QMessageBox::information(this,"Json File has been generated","Json file is generated successfully. Click [Open json file]");
    ui->openJSON_PushButton->setEnabled(1);
    ui->saveJSON_PushButton->setEnabled(1);
}


void MainWindow::on_openJSON_PushButton_clicked()
{
    text = json;
    Dialog modalDialog;
    modalDialog.setModal(true);
    modalDialog.exec();
}


void MainWindow::on_saveJSON_PushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");

           // An object for reading and writing files
           QFile file(fileName);

           // Try to open a file with write only options
           if (!file.open(QFile::WriteOnly | QFile::Text)) {
               QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
               return;
           }

           // Store the currentFile name
           currentFile = fileName;

           // Set the title for the window to the file name
           setWindowTitle(fileName);

           // Interface for writing text
           QTextStream out(&file);

           // Output to file
           out << json;
}

