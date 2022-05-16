#ifndef WMRDEVICE_H
#define WMRDEVICE_H

#include <QObject>
#include <QIODevice>
#include <deque>
#include <QDateTime>

using namespace std;

class WMRdevice : public QObject
{
    Q_OBJECT

    /*
     * type 4: thermometreMultiCanal
     */
public:

    /**
     * @brief temperature1
     * @return température de la sonde 1 en °C
     */
    float temperature1() const;
    /**
     * @brief temperature2
     * @return température de la sonde 2 en °C
     */
    float temperature2() const;
    /**
     * @brief temperature3
     * @return température de la sonde 3 en °C
     */
    float temperature3() const;
    /**
     * @brief batterieFaibleTemp1
     * @return True si la batterie de la sonde 1 est faible
     */
    bool batterieFaibleTemp1() const;
    /**
     * @brief batterieFaibleTemp2
     * @return True si la batterie de la sonde 2 est faible
     */
    bool batterieFaibleTemp2() const;
    /**
     * @brief batterieFaibleTemp3
     * @return True si la batterie de la sonde 3 est faible
     */
    bool batterieFaibleTemp3() const;

private:
    float mTemperature1;
    float mTemperature2;
    float mTemperature3;
    bool mBatterieFaibleTemp1;
    bool mBatterieFaibleTemp2;
    bool mBatterieFaibleTemp3;
    /**
     * @brief analyserTrameThermometreMultiCanal
     */
    void analyserTrameThermometreMultiCanal();

    /*
     *  type 0 : anémomètre
     */


    /*
     *  type 1 : pluviomètre
     */


    /*
     * type 5 : sonde intérieure (trame "BTH")
     */


    /*
     * type 3 : sonde extérieure de température, humidité et point de rosée (trame "Mushroom")
     */


    /*
     * type 14: minute
     * trame émise toutes les minutes
     */


    /*
     * type 15: horloge
     */



    /*
     * Ne pas modifier la suite
     */
public:
    explicit WMRdevice(QObject *parent = nullptr);
    /**
     * @brief setIoDevice
     * @param value un QIODevice connecté à la station météo
     */
    void setIoDevice(QIODevice *value);
    /**
     * @brief setTrame
     * @param newTrame La nouvelle trame à analyser
     */
    void setTrame(const deque<unsigned char> &newTrame);

private:
    enum Type{
        anemometre = 0,
        pluviometre = 1,
        sondeExterieure = 3,
        thermometreMultiCanal = 4,
        minutesSeules = 14,
        horloge = 15,
        sondeInterieure = 6
    };
    QIODevice *mIoDevice;
    deque<unsigned char> mBuffer;
    deque<unsigned char> mTrame;
    void analyserTrame();

    bool sommeControleEstValide();
    uint tailleSelonTypeTrame(uchar t);

public slots:
    void lireIo();

signals:
    void nouvelleDonneeRecue();
};

#endif // WMRDEVICE_H
