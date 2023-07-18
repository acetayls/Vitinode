/*****************************************
* 
* CHANGER LES VALEURS SUIVANTES selon la configuration !!!!
* 
/*****************************************/

// identifiant du node TTN
const char* devEui = "70B3D57ED0056DF3"; // Change to TTN Device EUI
const char* appEui = "0000000000000000"; // Change to TTN Application EUI
const char* appKey = "A175BEBB1955490D7E41FB3976966FFC"; // Change to TTN Application Key

// selection station meteo (uncomment le type en supprimant // et commenter les autres avec //)
//#define VITI_TYPE 1 //si vitbox : Sonde DS18B20 --> 1;
//#define VITI_TYPE 2 // si station météo : sonde DHT22 --> 2 ; 
//#define VITI_TYPE 3 // si station météo : sonde capa + DHT22 + pluvio --> 3 ; 
//#define VITI_TYPE 4 // si niveau eau : sonde HCSR04 --> 4
//#define VITI_TYPE 5 // si compteur énergie -> 5
#define VITI_TYPE 5 

// Temps entre les mesures (en secondes)
const unsigned long secs_between_send = 60*60; // en secondes ;ex: 60*60=1heure

// Coefficient de la batterie (il faut bien prendre 2 résistances identiques pour avoir E/2 en pin 36)
//avec E la tension délivrée par la batterie, le pont diviseur de tension (pdt) donne  E * R1/(R1+R2)=E *(R1/2R1)=E/2 au pin 36 si les R sont =
const double batt_coeff = 0.0019; // ( ((0,95/1000)*2*(E/2))  ) // si les resistance sont identiques et (0.95/1000)*2 facteur correctif de l'imprécision des résistances et corriger pdt et passer de mV en V
//pour l'avoir en %, car 4.2V la batterie au max=100%; Emin=3.2V; E V délivré = X% et on a E/2 au pin 36 -> produit en croix 

/*****************************************
* 
* FIN des valeurs à modifier !
* 
/*****************************************/
