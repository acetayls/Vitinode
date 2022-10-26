/*****************************************
* 
* CHANGER LES VALEURS SUIVANTE selon le configuration !!!!
* 
/*****************************************/

// identifiant du node TTN
const char* devEui = "70B3D57ED0056DF3"; // Change to TTN Device EUI
const char* appEui = "0000000000000000"; // Change to TTN Application EUI
const char* appKey = "A175BEBB1955490D7E41FB3976966FFC"; // Chaneg to TTN Application Key

// selection station meteo (2) ou vitibox (1)
#define VITI_TYPE 2 //si vitbox : 1 --> sonde DS18B20 sinon 2 --> sonde DHT22

// Temps entre les mesures (en secondes)
const unsigned long secs_between_send = 60*60;

// Coefficient de la batterie (en fonction des valeurs des resistances, mesure sur le terrain)
const double batt_coeff = 0.001611721612; // (3.3V * R1/(R1+R2) / 4095 ) // si les resistance sont identiques : 0.001074481
 
/*****************************************
* 
* FIN des valeurs à modifier !
* 
/*****************************************/