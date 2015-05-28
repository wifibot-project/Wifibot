#include "wifibot.h"
#include "ui_wifibot.h"
#include <QTextEdit>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include <QWebView>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTcpSocket>
#include <QTextEdit>
#include <QProgressBar>
#include <QSlider>
#include <QHBoxLayout>




/* Déclaration de vars */
QTimer *temps;
QTcpSocket socket;
QByteArray trame;
QString ip;
quint8 batterie,secteur,cap1,cap2,cap3,cap4;
quint16 VitesseR,VitesseL;
QNetworkAccessManager *gestionCam;
QString lienHaut,lienBas,lienDroite,lienGauche,lienInit1,lienInit2;
quint8 vLeft,vRight;
QSlider *vitL,*vitR;


wifibot::wifibot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wifibot)
{
    lienHaut="/?action=command&dest=0&plugin=0&id=10094853&group=1&value=-200";
    lienDroite="/?action=command&dest=0&plugin=0&id=10094852&group=1&value=-200";
    lienGauche="/?action=command&dest=0&plugin=0&id=10094852&group=1&value=200";
    lienBas="/?action=command&dest=0&plugin=0&id=10094853&group=1&value=200";
    lienInit1="/?action=command&dest=0&plugin=0&id=10094855&group=1&value=1";
    lienInit2="/?action=command&dest=0&plugin=0&id=10094854&group=1&value=1";

    //Instanciations
    gestionCam = new QNetworkAccessManager(this);
    temps = new QTimer(this);temps->setInterval(20);
    connect(temps,SIGNAL(timeout()),this,SLOT(Traitement()));


    ui->setupUi(this);
}

wifibot::~wifibot()
{
    delete ui;
}


   /* Connexion au serveur */
   void wifibot::connexionTCP(QString ip,int port){
   socket.connectToHost(ip, port);
   connect(&socket, SIGNAL(connected()),this, SLOT(Connecte()));
   //if (socket->waitForConnected(1000))

}



/* Lecture de données dans la socket */
void wifibot::Lecture()
{
    QDataStream Donnees(&socket);
    Donnees.setByteOrder(QDataStream::LittleEndian);
    quint16 speedL,speedR;
    quint8 battery,var1,var2,var3,c1,c2,c3,c4,Courant;
    float val1,val2;
    Donnees>>speedL>>battery>>c1>>c2>>val1>>speedR>>c3>>c4>>val2>>Courant>>var1>>var2>>var3;
    batterie=((battery/10));
    VitesseL=speedL;
    VitesseR=speedR;
    secteur=Courant/10;


    ui->bat->setValue(batterie);
    ui->Secteur->setValue(secteur);
    ui->VitesseG->setValue(VitesseL);
    ui->VitesseR->setValue(VitesseR);
}


/* Trame */

void wifibot::trameCons(quint8 vLeft, quint8 vRight, int dir)
{

    trame.clear();


    trame.append((char)0xff);
    trame.append((char)0x07);
    trame.append((quint8)vLeft);
    trame.append((char)(0x00));
    trame.append((quint8)vRight);
    trame.append((char)(0x00));
    if(dir==1){
        trame.append((char)0x00);
    }
    else if(dir==0){
        trame.append((char)0x50);
    }
    else if(dir==2){
        trame.append((char)0x40);
    }
    else if(dir==3){
        trame.append((char)0x10);
    }


    quint16 crc = Crc16(&trame, 1);
    trame.append((char)crc);
    trame.append((char)(crc>>8));

}




/* Gestion de la caméra */
void wifibot::on_camHaut_pressed()
{
    QUrl url("http://192.168.1.106:8080"+lienHaut);
            gestionCam->get(QNetworkRequest(url));
}

void wifibot::on_camBas_pressed()
{
    QUrl url("http://192.168.1.106:8080"+lienBas);
            gestionCam->get(QNetworkRequest(url));
}

void wifibot::on_camGauche_pressed()
{
    QUrl url("http://192.168.1.106:8080"+lienGauche);
            gestionCam->get(QNetworkRequest(url));
}

void wifibot::on_camDroite_pressed()
{
    QUrl url("http://192.168.1.106:8080"+lienDroite);
            gestionCam->get(QNetworkRequest(url));
}

void wifibot::on_initCam_pressed()
{
    QUrl url("http://192.168.1.106:8080"+lienInit1);
            gestionCam->get(QNetworkRequest(url));

    QUrl url2("http://"+ip+":8080"+lienInit2);
            gestionCam->get(QNetworkRequest(url2));
}







/* Gestion de la connexion */
void wifibot::on_connect_clicked()
{
    socket.connectToHost(ui->ip->text(),15020);
    connect(&socket, SIGNAL(connected()),this, SLOT(Connecte()));
    connect(&socket, SIGNAL(disconnected()),this, SLOT(Deconnecte()));
    temps->start();
}


/* Si connecté */
void wifibot::Connecte()
{
    ui->ip->setText("Connecté !");
    QString cam = "http://192.168.1.106:8080/javascript_simple.html";
    ui->rtmp->load(QUrl(cam));
    ui->rtmp->show();
    temps->start();
}

/* Traitement */
void wifibot::Traitement()
{
    socket.write(trame);

    QDataStream Donnees(&socket);
    Donnees.setByteOrder(QDataStream::LittleEndian);
    quint16 speedL,speedR;
    quint8 battery,var1,var2,var3,c1,c2,c3,c4,Courant;
    float val1,val2;
    Donnees>>speedL>>battery>>c1>>c2>>val1>>speedR>>c3>>c4>>val2>>Courant>>var1>>var2>>var3;
    batterie=((battery*0.1)*100)/12.7
            ;
    VitesseL=speedL;
    VitesseR=speedR;
    secteur=Courant/10;
    cap1=c1*3.3/255;
    cap2=c2*3.3/255;
    cap3=c3*3.3/255;
    cap4=c4*3.3/255;


    ui->bat->setValue(batterie);
    ui->Secteur->setValue(secteur);
    ui->VitesseG->setValue(VitesseL);
    ui->VitesseR->setValue(VitesseR);
    ui->cap1->setValue(cap1);
    ui->cap2->setValue(cap2);
    ui->cap3->setValue(cap3);
    ui->cap4->setValue(cap4);
}



/* Definition du CRC */
quint16 wifibot::Crc16(QByteArray* byteArray, int pos){
    unsigned char *data = (unsigned char* )byteArray->constData();
    quint16 crc = 0xFFFF;
    quint16 Polynome = 0xA001;
    quint16 Parity = 0;
    for(; pos < byteArray->length(); pos++){
        crc ^= *(data+pos);
        for (unsigned int CptBit = 0; CptBit <= 7 ; CptBit++){
            Parity= crc;
            crc >>= 1;
            if (Parity%2 == true) crc ^= Polynome;
        }
    }
    return crc;
}




void wifibot::on_btHaut_released()
{
    wifibot::trameCons(0,0,0);
}

void wifibot::on_btDroite_released()
{
    wifibot::trameCons(0,0,2);
}

void wifibot::on_btGauche_released()
{
    wifibot::trameCons(0,0,3);
}

void wifibot::on_btBas_released()
{
    wifibot::trameCons(0,0,1);
}

void wifibot::on_btHaut_pressed()
{

    if(cap1==0){
    wifibot::trameCons(ui->vitTest->value(),ui->vitTest->value(),0);
    }else{
        wifibot::trameCons(0,0,0);
    }

    wifibot::trameCons(ui->vitTest->value(),ui->vitTest->value(),0);
}

void wifibot::on_btDroite_pressed()
{
    wifibot::trameCons(-ui->vitTest->value(),ui->vitTest->value(),2);
}

void wifibot::on_btGauche_pressed()
{
    wifibot::trameCons(ui->vitTest->value(),-ui->vitTest->value(),3);
}

void wifibot::on_btBas_pressed()
{
    if(cap2==0){
    wifibot::trameCons(ui->vitTest->value(),ui->vitTest->value(),1);
    }else{
        wifibot::trameCons(0,0,1);
    }
}


void wifibot::on_pushButton_clicked()
{

    socket.disconnectFromHost();
}

void wifibot::Deconnecte()
{
   ui->ip->setText("Déconnecté");
}

void wifibot::on_photo_clicked()
{
    QString url = "http://127.0.0.1/projects/capture/";
    ui->capture->load(QUrl(url));
    ui->capture->show();
}

void wifibot::on_recorder_clicked()
{
    QString url2 = "http://127.0.0.1/projects/record/";
    ui->record->load(QUrl(url2));
    ui->record->show();
}
