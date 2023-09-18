// Librairie permettant d'effectuer les fonctions du servomoteur
#include<Servo.h>
Servo servoModule; // Définir le servomoteur utilisé

// Pins Trigger et Echo qui sont connectés au port du Arduino
const int trigUtilisateur = 10;
const int echoUtilisateur = 11;
const int trigDechet = 5;
const int echoDechet = 6;

// Initialisation du temps des impulsions des capteurs
long tempsDechet;
long tempsUtilisateur;
// Initialisation de la distance mesurée par les capteurs
int distanceDechet;
int distanceUtilisateur;
byte voix = 0; // Initialisation du terme (code) prononcé par l'utilisateur

void setup()
{
// Pin Echo correspond à une entrée et Trigger à une sortie
  pinMode(echoUtilisateur, INPUT);
  pinMode(trigUtilisateur, OUTPUT);
  pinMode(echoDechet, INPUT);
  pinMode(trigDechet, OUTPUT);

  Serial.begin(9600); // Initialisation à un taux de 9600 bits par seconde
  servoModule.attach(9); // Le servomoteur est connecté au port 9 du Arduino
  servoModule.write(0); // L'état initial du servomoteur est à 0 degré
  delay(2000); // Délai de 2 secondes
  
  Serial.write(0xAA);
  Serial.write(0x37); // Permet d'aller en mode compact pour le module de reconnaissance vocale
  delay(1000); // Delai de 1 seconde

  Serial.write(0xAA);
  Serial.write(0x21); // Importe le groupe 1 des voix sauvegardée dans le module de reconnaissance vocale
}

void loop()
{
  digitalWrite(trigDechet, LOW); // Transmission d'un impulsion à l'état bas au capteur ultrason
  delayMicroseconds(2); // Temps de cette impulsion
  digitalWrite(trigDechet, HIGH); // Tramsmission d'une impulsion à l'état haut au capteur ultrason
  delayMicroseconds(10); // Temps de cette impulsion

  digitalWrite(trigDechet, LOW); // L'impulsion retourne à l'état bas après ce temps
  tempsDechet = pulseIn(echoDechet, HIGH); // Temps des impulsions produit par la broche Echo du capteur des déchets
  distanceDechet = tempsDechet * 0.034 / 2; // Distance mesuré par le capteur des déchets
  
  while(distanceDechet > 10) // Tant que les déchets sont à plus de 10  cm du capteur des déchets on fait les 2 fonctions
  {
    parler();
    detecterCorps();
  }
}

// Affichage de la distance pour les tests
  Serial.print("La distance des déchets est: ");
  Serial.println(distanceDechet);
  Serial.print("La distance de l’utilisateur est: ");
  Serial.println(distanceUtilisateur);

// Fonction permettant d'ouvrir la poubelle par deux commandes vocales
void parler()
{
  while(Serial.available()) // Tant que le microphone a capté une voix
  {
  voix = Serial.read(); // La variable définit prend comme valeur le terme prononcée
  switch(voix) // Condition if pour savoir si la voix prononcée correspond à l'un des deux termes
{
  case 0x11: // Si le terme prononcé est égale à "open" (0x11)
  servoModule.write(180); // Le servomoteur effectue une rotation d 180 degrés.
  delay(3000);
  servoModule.write(10); // Le servomoteur revient à son état initial après 3 secondes
  break; // On sort de cette conditon

 case 0x12: // Si le terme prononcé est égale à "ouvrir" (0x12)
 servoModule.write(180);
 delay(3000);
 servoModule.write(0);
 break;
}
}
}

// Fonction permettant d'ouvrir la poubelle après avoir détecter le corps de l'utilisateur
void detecterCorps()
{
  digitalWrite(trigUtilisateur, LOW);
  delayMicroseconds(2);
  digitalWrite(trigUtilisateur, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigUtilisateur, LOW);
  tempsUtilisateur = pulseIn(echoUtilisateur, HIGH);
  distanceUtilisateur = tempsUtilisateur * 0.034 / 2;
  if(distanceUtilisateur > 10) // Si le capteur détecte l'utilisateur à une distance d'au plus de 10 cm
  {
  servoModule.attach(9);
  servoModule.write(180); // Le servomoteur effectue une rotation de 180 degrés.
  delay(3000);
  servoModule.write(0); // Le servomoteur revient à son état initial 3 secondes après que le capteur n'a rien détecté
}
}
