#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  socket = new QTcpSocket(this);

}

void MainWindow::timerEvent(QTimerEvent *e){ //reenvia a cada instante de tempo
    putData();
}

void MainWindow::putData() //envia textos com a data, tempo e número aleatório
{
   int maximo = ui->maximo->value(); //constante valor máximo
   int minimo = ui->minimo->value(); //constante valor mínimo

  QDateTime datetime;
  QString str;
  if(socket->state()== QAbstractSocket::ConnectedState){
      datetime = QDateTime::currentDateTime();
      str = "set "+
          datetime.toString(Qt::ISODate)+
          " "+
          QString::number((qrand()%((maximo+1)-minimo))+minimo)+"\r\n";

      qDebug() << str;
      qDebug() << socket->write(str.toStdString().c_str()) <<" bytes written";
      if(socket->waitForBytesWritten(3000)){
        qDebug() << "wrote";
      }
  }

  ui->textBrowser->append(str); //impressão no textBrowser

}

MainWindow::~MainWindow()
{
  delete socket;
  delete ui;
}

void MainWindow::conect() //conecta ao servidor
{
    socket->connectToHost(ui->servidor->text(),1234);
    if(socket->waitForConnected(3000)){
      qDebug() << "Conectado";
    }
    else{
      qDebug() << "Desconectado";
    }
}

void MainWindow::desconectar() //desconecta do servidor
{
    socket->close();
}

void MainWindow::iniciar()//inicia o Tempo
{
    int inicio = ui->timing->value(); //cria variável para receber o valor do tempo
    timer = startTimer(inicio*1000); //inicia o Tempo de acordo com o escolhido
}


void MainWindow::parar() //termina o Tempo quando clicado
{
    killTimer(timer);
}
