#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>
#include <QFile>
QUdpSocket *socket;
#define Buffer_size 1208
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket=new QUdpSocket(this);
    socket->bind(QHostAddress("192.168.12.177"),64826);
    connect(socket,SIGNAL(newConnection()),this,SLOT(new_connection()));
    qDebug()<<"connected";
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::new_connection()
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(on_pushButton_clicked()));
    qDebug()<<"new connection called";
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray Data;
    QByteArray buffer;
    QHostAddress sender;
    quint16 senderport;

    buffer.resize(socket->pendingDatagramSize());

    //Reading data
    socket->readDatagram(buffer.data(),buffer.size(),&sender,&senderport);

   //writing data
     Data.append("R");
    socket->writeDatagram(Data , QHostAddress("255.255.255.255"),65204);

    qDebug() << " from: " << sender.toString();
    qDebug() << "port: " << senderport;
    qDebug() << "the message : " << buffer;
    qDebug() << "size: " << buffer.size();

    qDebug() << "Pending datagrams: " << socket->hasPendingDatagrams();


    QString res = QString(buffer.toHex());

    //Displays recieving bytes
    qDebug() << res;

    char buff[Buffer_size];
   union
   {
        int X;
        char c[1];

   }un1;

   un1.c[0] = *(buffer.data()+1);
   un1.c[1] = 0;

   int frequency;
   int framelength;

   framelength=un1.X;

   if(framelength > Buffer_size)
        return;


   if(buffer.size() > Buffer_size)
        return;
    int i;
    int x=0;
    for(i=1;x<framelength;i++,x++)
    {
         buff[x] = *(buffer.data()+i);
     //    printf("%X ",buff[x]);
    }

    //save data into file
    QFile File("F:/Data.bin");
    if(File.open(QFile::WriteOnly|QFile::Append))
    {

        QDataStream out(&File);
        out<<buff[8]<<res;

    }
}
