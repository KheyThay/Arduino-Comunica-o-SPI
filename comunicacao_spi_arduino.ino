// Bibliotecas incluídas 
#include <LiquidCrystal.h>

// Definições de pinos do Arduino 
#define CLK 7
#define SS 8
#define MOSI 9
#define MISO 10

// Variáveis
int clkState = LOW;
int prevClkState = LOW;
byte data = 0x00; // para armazenar os bits recebidos
int bitPos = 3; // posição do bit atual
unsigned long timerStart;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
    // configurar pinos como entrada ou saída
    pinMode(CLK, INPUT);
    pinMode(MOSI, INPUT);
    pinMode(MISO, OUTPUT);
    pinMode(SS, INPUT);
  
    // definir estados iniciais dos pinos de saída
    digitalWrite(MISO, LOW);
  
    // configurar o número de colunas e linhas do LCD
    lcd.begin(16, 2);
    lcd.setCursor(0, 1);  
}  

void loop() {
    clkState = digitalRead(CLK); // alinhar o relógio com o relógio do mestre
    if (clkState != prevClkState) // se o relógio mudou
    {
        prevClkState = clkState;
        if (digitalRead(SS) == LOW) // e se este escravo deve receber uma mensagem
        {
            if (clkState == HIGH) // e se a mudança de relógio foi de baixo para cima
            {
                if (digitalRead(MOSI) == LOW) // se MOSI estava baixo, registrar um 0 na posição do bit atual
                {
                    data &= ~(0x01 << bitPos); // 'AND' o bit existente com 0 para torná-lo 0
                    bitPos--; // decrementar para a próxima posição do bit
                }
                else // se MOSI estava alto, registrar um 1 na posição do bit atual
                {
                    data |= (0x01 << bitPos); // 'OR' o bit existente com 1 para torná-lo 1
                    bitPos--; // decrementar para a próxima posição do bit
                }
                if (bitPos < 0) // se todos os 4 bits foram definidos
                {    
                    delay(500);
                    digitalWrite(MISO, HIGH); // enviar um sinal de confirmação
                    delay(1000);
                    digitalWrite(MISO, LOW);
                    bitPos = 3; // redefinir a posição do bit
                    lcd.clear(); // limpar os dados anteriores no LCD
                    lcd.print(data); // enviar dados para o LCD
                }
            }
        }
    }
  
    delay(10); // Atraso para melhorar o desempenho da simulação
} 
// fim do loop