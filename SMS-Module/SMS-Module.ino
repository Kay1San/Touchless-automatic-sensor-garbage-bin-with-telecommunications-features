// Librarie pour utiliser les fonctions du module 3G
#include <MKRGSM.h>
const char PIN[] = "1234"; // Numéro de pin de la carte SIM

// Variables pour la connexion au réseau 3G
GSM gsm;
GSM_SMS sms;

// Connexion des pins au Arduino
const int trigDechet = 6;
const int echoDechet = 7;
const int rougeLED = A1;
const int vertLED = A2;

// Initialisation du temps des impulsions des capteurs
long tempsDechet;
int distanceDechet;

// Initialisation d'un compteur pour le nombre de fois qu'un SMS est envoyé
int SMSsent = 0;

void setup() {
// Définition des entrées et des sorties des pins
  pinMode(echoDechet, INPUT);
  pinMode(trigDechet, OUTPUT);
  pinMode(rougeLED, OUTPUT);
  pinMode(vertLED, OUTPUT);

  Serial.begin(9600); // Initialisation à un taux de 9600 bits par seconde
  bool connected = false; // Initialisation de la connexion au module 3G
  if (gsm.begin(PIN) == GSM_READY) // Si le numéro de pin s'est connecté au réseau 3G
  {
    connected = true; // La connexion au réseau 3G est conforme
  }
  else
  {
    Serial.println("Module 3G n'est pas connecté"); // Sinon on affiche ce terme dans la console pour vérifier que le module 3G n'est pas connecté
    delay(1000);
  }
  Serial.println("Module iniatilisé"); // Affichage de ce terme dans la console pour démontrer que le module est iniatilisé
}


void allumerRougeLED() // Fonction permettant d'allumer la DEL rouge
{
  if (distanceDechet <= 7) // Si la distance mesuré par le capteur des déchets est inférieure ou égale à 7 cm
{
 digitalWrite(rougeLED, HIGH); // On applique une tension de 2V à la DEL rouge
}
  else
  {
  digitalWrite(rougeLED, LOW); // Sinon on la laisse à l'état bas
  }
  delay(100);
  }
 
void allumerVertLED() // Fonction permettant d'allumer la DEL verte
{
  if (distanceDechet > 7)
  {
    digitalWrite(vertLED, HIGH);
   }
  else
  {
    digitalWrite(vertLED, LOW);
  }
  delay(100);
}

void loop() {
  digitalWrite(trigDechet, LOW); // Transmission d'un impulsion à l'état bas au capteur ultrason
  delayMicroseconds(2); // Temps de cette impulsion
  
  digitalWrite(trigDechet, HIGH); // Tramsmission d'une impulsion à l'état haut au capteur ultrason
  delayMicroseconds(10); // Temps de cette impulsion
  
  digitalWrite(trigDechet, LOW); // L'impulsion retourne à l'état bas après ce temps
  tempsDechet = pulseIn(echoDechet, HIGH); // Temps des impulsions produit par la broche Echo du capteur des déchets

distanceDechet = tempsDechet * 0.034 / 2; // Distance mesuré par le capteur des déchets

Serial.print("La distance est: ");
Serial.println(distanceDechet); // Affichage de la distance dans la console pour les tests

// On effectue les deux fonctions permettant d'allumer les DEL en boucle
 allumerRougeLED();
 allumerVertLED();
 
 char remoteNum[]="5148059186"; // Définir le numéro de téléphone du cellulaire auquel le SMS sera envoyé
  if (distanceDechet <= 7) // Si la distance mesuré par le capteur est inférieure ou égale à 7 cm
  {
    // Définir le message qui sera envoyé
    char textMsg[]="La poubelle est remplie, veuillez la vider";
    if(SMSsent == 0) // Compteur : si aucun SMS n'a précédemment été envoyé
    {
      sms.beginSMS(remoteNum); // On envoie le SMS au téléphone
      sms.print(textMsg);
      sms.endSMS();
      Serial.println("\nCOMPLETE!\n"); //Affichage de la réussite de l'envoie
      dans la console
      SMSsent = 1; // Le compteur est à 1, donc aucun SMS ne pourra être
      envoyé
    }
  }
  else
  {
    SMSsent = 0; // On remet le compteur à 0 lorsque la poubelle est vider pour envoyer un deuxième SMS lorsqu’elle sera remplie à nouveau

  }
}
