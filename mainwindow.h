#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QHash>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_notes_currentTextChanged(const QString &currentText);

    void on_actionOpen_triggered();

    void on_notes_doubleClicked(const QModelIndex &index);

    void on_add_clicked();

    void on_actionSave_triggered();

    void on_del_clicked();


private:
    Ui::MainWindow *ui;
    void load_notes(QString filename);
    QStringList words;
    QString name;
    QHash<QString, QString> notextts;
};
#endif // MAINWINDOW_H
