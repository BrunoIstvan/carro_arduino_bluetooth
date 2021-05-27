
//INCLUSAO DE BIBLIOTECA DO CONTROLE SERIAL 
#include <SoftwareSerial.h>

// PINAGEM DO MOTOR DIREITO
#define PIN_VELOCIDADE_MOTOR_DIREITO  9
#define PIN_1_MOTOR_DIREITO           11
#define PIN_2_MOTOR_DIREITO           10

// PINAGEM DO MOTOR ESQUERDO
#define PIN_VELOCIDADE_MOTOR_ESQUERDO 5
#define PIN_2_MOTOR_ESQUERDO          6
#define PIN_1_MOTOR_ESQUERDO          7

// TEMPO PADRAO QUE IRA GIRAR OS MOTORES AO IREM PARA FRENTE E PARA TRAS
#define TEMPO                         600

// VARIAVEL QUE CONTROL A VELOCIDADE. ELA PODE SER ALTERADA UTILIZANDO OS PINOS PWM DO L298N
uint8_t velocidade                    = 0;
// ESSA VARIAVEL AUXILIA PARA EQUILIBRAR A VELOCIDADE DE UM DOS MOTORES PARA O CASO DE 
//  UM DOS MOTORES RODAR MENOS QUE O OUTRO
uint8_t velocidade_ajuste             = 18;

const char BOTAO_CIMA                 = 'w'; 
const char BOTAO_BAIXO                = 's'; 
const char BOTAO_ESQUERDA             = 'a';
const char BOTAO_DIREITA              = 'd';
const char BOTAO_PARAR                = 'q';
const char BOTAO_1                    = '1';
const char BOTAO_2                    = '2';
const char BOTAO_3                    = '3';

// VELOCIDADE MINIMA, MEDIA E MAXIMA DOS MOTORES QUE EU DEFINI, ABAIXO DE 65 O MOTOR NAO TEM FORCA PARA RODAR
const uint8_t MIN_VELOCIDADE          = 75;
const uint8_t MED_VELOCIDADE          = 95;
const uint8_t MAX_VELOCIDADE          = 115;

uint8_t state                         = 0;  // Variável para avaliar os comando recebidos

// COMUNICACAO SERIAL
SoftwareSerial HC05(12, 13); // Simula TX e RX nas portas 12 e 13

// MOVE PARA FRENTE E PARA
void mover_frente() {
  digitalWrite(PIN_1_MOTOR_DIREITO, HIGH);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, HIGH);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  delay(TEMPO);
  parar();
}

// MOVE PARA TRAS E PARA
void mover_tras() {
  digitalWrite(PIN_1_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_2_MOTOR_DIREITO, HIGH);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, LOW);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, HIGH);
  delay(TEMPO);
  parar();
}

// MOVE PARA A DIREITA E PARA
void mover_direita() {
  digitalWrite(PIN_1_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, HIGH);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  // AQUI DEFINI DIVIDIR POR 3, CASO QUEIRA QUE O CARRO VIRE MAIS OU MENOS, DIMINUA ESSA DIVISAO POR 3 OU AUMENTE, RESPECTIVAMENTE
  delay(TEMPO / 3);
  parar();
}

// MOVE PARA A DIREITA E PARA
void mover_esquerda() {
  digitalWrite(PIN_1_MOTOR_DIREITO, HIGH);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, LOW);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  // AQUI DEFINI DIVIDIR POR 3, CASO QUEIRA QUE O CARRO VIRE MAIS OU MENOS, DIMINUA ESSA DIVISAO POR 3 OU AUMENTE, RESPECTIVAMENTE
  delay(TEMPO / 3);
  parar();
}

// PARA OS MOTORES
void parar() {
  digitalWrite(PIN_1_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_2_MOTOR_DIREITO, LOW);
  digitalWrite(PIN_1_MOTOR_ESQUERDO, LOW);
  digitalWrite(PIN_2_MOTOR_ESQUERDO, LOW);
  //delay(TEMPO);
}

void setup() {
  
  HC05.begin(38400);      // TAXA DE TRANSMISSAO DO MODULO
  Serial.begin(9600);     // INICIALIZA A SERIAL

  // CONFIGURA AS PINAGENS DO L298N
  // MOTOR DIREITO
  pinMode(PIN_VELOCIDADE_MOTOR_DIREITO, OUTPUT);
  pinMode(PIN_1_MOTOR_DIREITO, OUTPUT);
  pinMode(PIN_2_MOTOR_DIREITO, OUTPUT);
  // MOTOR ESQUERDO
  pinMode(PIN_VELOCIDADE_MOTOR_ESQUERDO, OUTPUT);
  pinMode(PIN_1_MOTOR_ESQUERDO, OUTPUT);
  pinMode(PIN_2_MOTOR_ESQUERDO, OUTPUT);

  // INICIO NA VELOCIDADE MINIMA
  velocidade                          = MIN_VELOCIDADE;

}

void mover(char value) {

  analogWrite(PIN_VELOCIDADE_MOTOR_DIREITO, velocidade);
  analogWrite(PIN_VELOCIDADE_MOTOR_ESQUERDO, velocidade + velocidade_ajuste);
  
  // A MINHA IDEIA AQUI ERA DEIXAR O VEICULO ANDANDO ATE ELE RECEBER O SINAL DE PARAR PELO CONTROLE.
  // PARA ISSO, RETIRE A LINHA DE DELAY() E PARAR() DENTRO DOS METODOS QUE MOVIMENTAR O MOTOR PARA FRENTE E PARA TRAS
  if (value == BOTAO_CIMA) {
    Serial.print("para frente");
    mover_frente();
  } else if (value == BOTAO_BAIXO) { 
    Serial.print("para baixo");
    mover_tras();
  } else if (value == BOTAO_ESQUERDA) { 
    Serial.print("para esquerda");
    mover_esquerda();
  } else if (value == BOTAO_DIREITA) { 
    Serial.print("para direita");
    mover_direita();
  } else if (value == BOTAO_PARAR) { 
    Serial.print("parar");
    parar();
  } else if (value == BOTAO_1) { 
    velocidade = MIN_VELOCIDADE;
  } else if (value == BOTAO_2) { 
    velocidade = MED_VELOCIDADE;
  } else if (value == BOTAO_3) {
    velocidade = MAX_VELOCIDADE;
  } // if (results.value == BOTAO_CIMA) {

}

void loop() {

  if (HC05.available() > 0) { // Se o sinal recebido no HC05 for maior que 0
    state = HC05.read(); // state será igual ao valor de mySerial
    char c = state;
    Serial.println(c);
    mover(c);
  }

  delay(100);
  
}
  
