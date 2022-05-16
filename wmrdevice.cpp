#include <QTextStream>
#include "wmrdevice.h"

WMRdevice::WMRdevice(QObject *parent) : QObject(parent)
{
    this->mIoDevice = nullptr;
    this->mBuffer.clear();
    this->mTrame.clear();

    /*
     * type 4: thermometreMultiCanal
     */
    this->mTemperature1 = 99999;
    this->mTemperature2 = 99999;
    this->mTemperature3 = 99999;
    this->mBatterieFaibleTemp1 = false;
    this->mBatterieFaibleTemp2 = false;
    this->mBatterieFaibleTemp3 = false;

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

}

/*
 * type 4: thermometreMultiCanal
 */
float WMRdevice::temperature1() const
{
    return this->mTemperature1;
}

float WMRdevice::temperature2() const
{
    return this->mTemperature2;
}

float WMRdevice::temperature3() const
{
    return this->mTemperature3;
}

bool WMRdevice::batterieFaibleTemp1() const
{
    return this->mBatterieFaibleTemp1;
}

bool WMRdevice::batterieFaibleTemp2() const
{
    return this->mBatterieFaibleTemp2;
}

bool WMRdevice::batterieFaibleTemp3() const
{
    return this->mBatterieFaibleTemp3;
}


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


void WMRdevice::analyserTrame()
{   
    switch (this->mTrame[2]) {
    case WMRdevice::thermometreMultiCanal :
        this->analyserTrameThermometreMultiCanal();
        break;
    case WMRdevice::anemometre :

        break;
    case WMRdevice::pluviometre :

        break;
    case WMRdevice::sondeExterieure :

        break;
    case WMRdevice::horloge :

        break;
    case WMRdevice::minutesSeules :

        break;
    case WMRdevice::sondeInterieure :

        break;
    }
    emit nouvelleDonneeRecue();
}

/*
 * type 4: thermometreMultiCanal
 */
void WMRdevice::analyserTrameThermometreMultiCanal()
{
    uchar canal          = this->mTrame[3] & 0b00000111;
    bool batterieFaible = (this->mTrame[3] & 0b01000000) == 0b01000000;
    uchar tempDixieme    = this->mTrame[4] & 0x0f;
    uchar tempUnite      = this->mTrame[4] >> 4;
    uchar tempDizaine    = this->mTrame[5] & 0x0f;
    uchar tempCentaine = ( this->mTrame[5] & 0x30 ) >> 4;
    uchar depassement    = this->mTrame[5] & 0x40;
    uchar signeNegatif   = this->mTrame[5] & 0x80;
    // calcul de la température
    double temp = tempCentaine*100 + tempDizaine*10 + tempUnite + tempDixieme*0.1;
    if (depassement) temp = 99999;
    if (signeNegatif) temp = -1 * temp;
    switch (canal) {
    case 0b00000001 :
        this->mTemperature1 = temp;
        this->mBatterieFaibleTemp1 =  batterieFaible;
        break;
    case 0b00000010 :
        this->mTemperature2=temp;
        this->mBatterieFaibleTemp2 =  batterieFaible;
        break;
    case 0b00000100 :
        this->mTemperature3=temp;
        this->mBatterieFaibleTemp3 =  batterieFaible;
        break;
    }
}


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

void WMRdevice::setIoDevice(QIODevice *value)
{
    if (this->mIoDevice) {
        this->mIoDevice->close();
    }
    this->mIoDevice = value;
    connect(this->mIoDevice,SIGNAL(readyRead()),this,SLOT(lireIo()));
    this->mIoDevice->open(QIODevice::ReadOnly);
}

void WMRdevice::setTrame(const deque<unsigned char> &newTrame)
{
    mTrame = newTrame;
    if (this->sommeControleEstValide()) this->analyserTrame();
}

bool WMRdevice::sommeControleEstValide()
{
    uchar checksum = 0;
    if (this->mTrame.size()==0) return false;
    for(uint i=0; i<this->mTrame.size()-1; i++)
        checksum = checksum + this->mTrame[i];

    if (this->mTrame.back() != checksum) {
            QTextStream ts( stdout );
            ts << "Somme de controle invalide : " << QString("%1").arg(checksum,2,16,QLatin1Char('0')) << "\n\n";
    }
    //return true;
    return (this->mTrame.back() == checksum);
}

uint WMRdevice::tailleSelonTypeTrame(uchar t) {
    uint tailleTrame = 0;
    switch (t) {

    case WMRdevice::anemometre :
        tailleTrame=11;
        break;
    case WMRdevice::pluviometre :
        tailleTrame=16;
        break;
    case WMRdevice::sondeExterieure :
    case WMRdevice::horloge :
        tailleTrame=9; //9 is OK , 15 is for debug
        break;
    case WMRdevice::thermometreMultiCanal :
        tailleTrame=7;
        break;
    case WMRdevice::minutesSeules :
        tailleTrame=5;
        break;
    case WMRdevice::sondeInterieure :
        tailleTrame=14;
        break;
    default:
        tailleTrame=0; // type errone
    }
    return tailleTrame;
}

void WMRdevice::lireIo()
{
    while (! this->mIoDevice->atEnd()) {
        char c;
        if (this->mBuffer.size()>0 && this->mBuffer[0] != 0xff) this->mBuffer.clear();
        this->mIoDevice->getChar(&c);
        uchar b = static_cast<unsigned char>(c);
        if (this->mBuffer.size()<2 && ( b !=0xff)) {  // on ne détecte pas un entête valide :
            this->mBuffer.clear();
            continue;
        }
        this->mBuffer.push_back(b);
    }

    do {
        if (this->mBuffer.size()<3) return;

        unsigned int tailleTrame = this->tailleSelonTypeTrame(this->mBuffer[2]);

        if (tailleTrame == 0) {  // trame de type erroné, donc erreur. On supprime jusqu'à détecter le début d'un nouvel entête.
            this->mBuffer.pop_front();
            this->mBuffer.pop_front();
            while (this->mBuffer.size()>0 && this->mBuffer[0]!=0xff) this->mBuffer.pop_front();
        }

        if (this->mBuffer.size() < tailleTrame) return; // la trame n'est pas encore complète

        // Trame complète !
        this->mTrame.clear();
        for (uint i=0;i<tailleTrame; i++) {
            this->mTrame.push_back(mBuffer[0]);
            this->mBuffer.pop_front();
        }
        if (this->sommeControleEstValide()) this->analyserTrame();
    } while (this->mBuffer.size()>0);
}
