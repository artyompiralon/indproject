#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QPalette>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)


{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::load_notes(QString filename)  //подгрузка файла в окно заполнение хеш
{
    notextts.clear(); //предварительная очистка для отсутствия визуальных багов
    words.clear();
    ui->notes->clear();
    ui->notetext->clear();
    ui->input->clear();


    QFile notelist(filename); //открытие фаила
    if(!notelist.open(QIODevice::ReadOnly)){
        return;
    }

    QTextStream stream(&notelist);
    QString line;
    QString word;
    QString transl;

    while (!stream.atEnd()){ //пока поток фаила не кончился
        line = stream.readLine();
        if (!line.isEmpty()&&!line[0].isSpace()){
            if(!word.isEmpty()){
              words.append(word); //запись значений название.текст в хеш
              notextts.insert(word, transl);
        }
            word = line.simplified();
            transl = 0;
        }else{
            line.remove(0,1);
            transl = transl + line +QString("\n");
        }}
        if(!word.isEmpty()){
          words.append(word);
          notextts.insert(word, transl);
    }
    notelist.close();
    ui->notes->addItems(words); //вывод списка названий
}

void MainWindow::on_notes_currentTextChanged(const QString &currentText) // если нажимаем на название заметки
{
    ui->notetext->setPlainText(notextts[currentText]); //вывод текста
    ui->input->setText(currentText); //вывод в строку


}

void MainWindow::on_actionOpen_triggered()
{
    QString f=("*.dict;;*.txt"); //фильтр подходящих форматов
    name = QFileDialog::getOpenFileName(this, "Выбор файла с заметками",f); //получаем имя фаила в диалоговом окне
    load_notes(name);   //передаём имя в функцию отображения
}


void MainWindow::on_notes_doubleClicked(const QModelIndex &index) //на двойное нажатие очищаем текст и строку
{
    ui->notetext->clear();
    ui->input->clear();

}

void MainWindow::on_add_clicked() //проверка на сущ значений . на повтор //запись новой заметки в фаил
{

    QString newword= 0;
    QString newtransl= 0;
    newword = ui->input->text(); //берем слово из строки
    newtransl = " " + ui->notetext->toPlainText(); //берем текст заметки и добавляем пробел для соблюдения формата словаря
    QString newwordc = newword.simplified();
    QString newtranslc = newtransl.simplified();

 for(auto i=0;i<words.size();++i){
 if (words[i]==newwordc){
     QMessageBox::warning(this,"Ошибка","Обнаружены одинаковые имена заметок");
     return;}
 }

    if(!newwordc.isEmpty()&&!newtranslc.isEmpty()){ //проверка на существующее значение с учетом возможности строк из пробелов.переводов
          QFile filename(name);

                if (filename.open(QIODevice::Append)) { //вставляем в файл

                        words.append(newwordc); //запись данных
                        notextts.insert(newwordc,newtransl);
                         QTextStream writeStream(&filename);
                            writeStream <<newwordc<<'\n';
                            writeStream <<newtransl<<'\n';



                 }else{
                    QMessageBox::warning(this,"Ошибка","Фаил с заметками не открыт");
                    return;}

     filename.close();
     load_notes(name);

    }else{QMessageBox::warning(this,"Ошибка","Форма заметки не заполнена");} //окно с предупреждением


     }

void MainWindow::on_actionSave_triggered() //на сохранение
{
     QString filename = QFileDialog::getSaveFileName(this, "Cохранить новый фаил с заметками",  tr ("notes.dict"));
     QFile f(filename);
      QFile f2(name);
     f2.open( QIODevice::ReadOnly ); //открываем текущий фали, направляем поток из него в новый записываемый
     f.open( QIODevice::WriteOnly );
     QTextStream stream2( &f2 );
      QTextStream stream( &f );
     stream << stream2.readAll();
     f2.close();
     f.close();
}


void MainWindow::on_del_clicked()
{
   QString delword;
      QString deltransl;
     delword = ui->input->text();
     deltransl = ui->notetext->toPlainText();
     notextts.remove(delword);
     words.removeAll(delword);

     QFile filename(name);  //перезапись файла
     if(filename.open(QIODevice::ReadWrite | QIODevice::Text))
     {
         QString s;
         QTextStream t(&filename);
         while(!t.atEnd())
         {

             QString line = t.readLine();
             if(!line.contains(delword))
                 s.append(line + "\n");
             else{
                 if(line.startsWith(" "))
                     s.append(line + "\n");
                     else{
                 t.readLine();}
             }
         }
         filename.resize(0);
         t << s;
         filename.close();

     }else{QMessageBox::warning(this,"Ошибка","Фаил с заметками не открыт");}
filename.close();

     load_notes(name);

}
