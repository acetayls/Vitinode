/*****************************************
* 
* CHANGER LES VALEURS SUIVANTES selon la configuration !!!!
* 
*****************************************/

// identifiant du node TTN
const char* devEui = "70B3D57ED005E53A"; // Change to TTN Device EUI
const char* appEui = "0000000000000000"; // Change to TTN Application EUI
const char* appKey = "C082FB519D4C9EDE56A382B2DA03B201"; // Chaneg to TTN Application Key

// sélection du capteur connecté (uncomment le type en supprimant // et commenter les autres avec // (Ctrl+/) )
//#define VITI_TYPE 1 // si VINIBOX (température cuve): Sonde DS18B20 --> 1
//#define VITI_TYPE 2 // si VITINODE (station météo T°/hygro air): sonde DHT22 --> 2
//#define VITI_TYPE 3 // si VITISOL (hygro du sol): sonde capa --> 3 
//#define VITI_TYPE 4 // si VITINODEXTEND (station météo T°/hygro air+sol): sonde capa + DS18B20 + DHT22 --> 4 
//#define VITI_TYPE 5 // si AGRISONAR (niveau d'eau) : sonde HCSR04 --> 5
//#define VITI_TYPE 6 // si VINIPOWER (compteur d'énergie) : EASTRON SDM530 --> 6
//#define VITI_TYPE 7 // si VITIPLUIE (pluviomètre): MS-WH-SP-RG --> 7
#define VITI_TYPE 5

// Temps entre les mesures (en secondes) pour VITITYPE 1,2,3,4,5
const unsigned long secs_between_send = 60*0.25;
// Temps entre les mesures (en secondes) pour VITITYPE 6,7
const unsigned long time_between_send = 1000*15; // temps entre les envois / mesure  ; en millisecondes

// Coefficient de la batterie (il faut bien prendre 2 résistances identiques pour avoir E/2 en pin 36)
//avec E la tension délivrée par la batterie, le pont diviseur de tension (pdt) donne  E * R1/(R1+R2)=E *(R1/2R1)=E/2 au pin 36 si les R sont =
const double batt_coeff = 0.0019; // ( ((0,95/1000)*2*(E/2))  ) // si les resistance sont identiques et (0.95/1000)*2 facteur correctif de l'imprécision des résistances et corriger pdt et passer de mV en V
//pour l'avoir en %, sur batterie lipo, car 4.2V la batterie au max=100%; Emin=3.2V; E V délivré = X% et on a E/2 au pin 36 si R= -> produit en croix 
 
/*****************************************
* 
* FIN des valeurs à modifier !
* 
*****************************************/
