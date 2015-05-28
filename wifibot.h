#ifndef WIFIBOT_H
#define WIFIBOT_H
#include <QMainWindow>
#include <QString>

namespace Ui {
class wifibot;
}

class wifibot : public QMainWindow
{
    Q_OBJECT

public:
    explicit wifibot(QWidget *parent = 0);
    ~wifibot();

    void connexionTCP(QString,int);

    quint16 Crc16(QByteArray*, int);

    void trameCons(quint8,quint8,int);





private slots:

    void Connecte();
    void Deconnecte();

    void Lecture();

    void Traitement();

    void on_camHaut_pressed();

    void on_camBas_pressed();

    void on_camGauche_pressed();

    void on_camDroite_pressed();

    void on_initCam_pressed();

    void on_connect_clicked();


    void on_btHaut_released();

    void on_btDroite_released();

    void on_btGauche_released();

    void on_btBas_released();

    void on_btHaut_pressed();

    void on_btDroite_pressed();

    void on_btGauche_pressed();

    void on_btBas_pressed();



    void on_pushButton_clicked();

    void on_photo_clicked();

    void on_recorder_clicked();

private:
    Ui::wifibot *ui;
};

#endif // WIFIBOT_H
