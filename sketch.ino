#include <LiquidCrystal.h>

// Pinos do sensor HC-SR04
const int TRIG_PIN = 8;
const int ECHO_PIN = 9;

// Pinos do buzzer
const int BUZZER_PIN = 10;

// Pinos do LCD
const int RS = 12;
const int EN = 11;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

// Inicializa o LCD (16 colunas, 2 linhas)
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// Variáveis
float distancia = 0;
unsigned long tempoEcho = 0;
int frequencia = 0;

void setup() {
  // Inicializa comunicação serial (para debug)
  Serial.begin(9600);
  
  // Configura pinos do sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Configura pino do buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Inicializa LCD
  lcd.begin(16, 2);
  lcd.print("Sistema Pronto!");
  delay(2000);
  lcd.clear();
  
  // Mensagem inicial no LCD
  lcd.print("Distancia: ");
  lcd.setCursor(0, 1);
  lcd.print("cm");
}

void loop() {
  // Mede a distância
  distancia = medirDistancia();
  
  // Calcula a frequência do buzzer baseado na distância
  // Quanto menor a distância, maior a frequência
  if (distancia > 0 && distancia < 400) {
    // Mapeamento: 2cm = 2000Hz, 100cm = 100Hz
    frequencia = map(distancia, 2, 100, 2000, 100);
    frequencia = constrain(frequencia, 100, 2000);
    
    // Toca o buzzer
    tone(BUZZER_PIN, frequencia, 50);
  } else {
    // Sem bip se fora do alcance
    noTone(BUZZER_PIN);
  }
  
  // Atualiza o LCD
  atualizarLCD(distancia);
  
  // Print no serial para debug
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm | Frequencia: ");
  Serial.print(frequencia);
  Serial.println(" Hz");
  
  delay(100); // Aguarda antes da próxima leitura
}

// Função para medir a distância usando HC-SR04
float medirDistancia() {
  // Envia pulso para o sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Lê o tempo do echo
  tempoEcho = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout de 30ms
  
  // Calcula a distância (velocidade do som = 340 m/s)
  // Distância = (tempo em microsegundos * velocidade do som) / 2
  // Distância em cm = (tempoEcho * 0.034) / 2 = tempoEcho * 0.017
  float dist = (tempoEcho * 0.034) / 2.0;
  
  // Se não houver leitura válida, retorna -1
  if (tempoEcho == 0) {
    return -1;
  }
  
  return dist;
}

// Função para atualizar o LCD
void atualizarLCD(float dist) {
  lcd.setCursor(11, 0); // Posição após "Distancia: "
  
  if (dist > 0) {
    // Limpa o espaço anterior e mostra a nova distância
    lcd.print("     ");
    lcd.setCursor(11, 0);
    lcd.print(dist, 1); // 1 casa decimal
  } else {
    // Mostra "---" se sem leitura
    lcd.print("---  ");
  }
}
