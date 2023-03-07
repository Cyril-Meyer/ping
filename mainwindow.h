#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include <QDebug>

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
    void on_pushButton_ping_clicked();

    void on_checkBox_ping_stateChanged(int);

    void on_spinBox_freq_valueChanged(int);

private:
    Ui::MainWindow *ui;
    QTimer *ping_timer;

    /*
     * learn.microsoft.com/en-us/windows/win32/api/icmpapi/nf-icmpapi-icmpsendecho
     */
    void newPing();
    long ping();
};
#endif // MAINWINDOW_H
