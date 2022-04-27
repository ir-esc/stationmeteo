#include "wmrdevice.h"
#include "iostream"
using namespace std;

#define EGAL(a,b) (abs(a-b)<0.01)

void test1() {
    cout << "Test trame 1" << endl;

    WMRdevice meteoDevice;
    deque<unsigned char> trame={0xff, 0xff, 0x04, 0x41, 0x03, 0x81, 0xc7};
    meteoDevice.setTrame(trame);

    if ( ! EGAL(meteoDevice.temperature1() , -10.3) ) cout << " - erreur temp 1" << endl ;
    if ( ! meteoDevice.batterieFaibleTemp1() )        cout << " - erreur batt 1" << endl;
    if ( ! EGAL(meteoDevice.temperature2() , 99999) ) cout << " - erreur temp 2" << endl;
    if (   meteoDevice.batterieFaibleTemp2() )        cout << " - erreur batt 2" << endl;
    if ( ! EGAL(meteoDevice.temperature3() , 99999) ) cout << " - erreur temp 3" << endl;
    if (   meteoDevice.batterieFaibleTemp3() )        cout << " - erreur batt 3" << endl;
}

void test2() {
    cout << "Test trame 2" << endl;

    WMRdevice meteoDevice;
    deque<unsigned char> trame={0xff, 0xff, 0x04, 0x04, 0x27, 0x03, 0x30};
    meteoDevice.setTrame(trame);

    if ( ! EGAL(meteoDevice.temperature1() , 99999) ) cout << " - erreur temp 1" << endl ;
    if (   meteoDevice.batterieFaibleTemp1() )        cout << " - erreur batt 1" << endl;
    if ( ! EGAL(meteoDevice.temperature2() , 99999) ) cout << " - erreur temp 2" << endl;
    if (   meteoDevice.batterieFaibleTemp2() )        cout << " - erreur batt 2" << endl;
    if ( ! EGAL(meteoDevice.temperature3() , 32.7) )  cout << " - erreur temp 3" << endl;
    if (   meteoDevice.batterieFaibleTemp3() )        cout << " - erreur batt 3" << endl;
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    test1();
    test2();
}
