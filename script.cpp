// C++ code
//
//importando biblioteca para usar o LCD

#include <Adafruit_LiquidCrystal.h>



Adafruit_LiquidCrystal lcd(0);


int tempSensor = A1; //colocando a entrada do TMP
int sensorInput;
double temp;
float temperatura = 0;
int RawValue= 0;
double Voltage = 0;//voltagem que o tmp 36 mostra de inicio
double tempC = 0;//valor da temperatura final
int altoFalante = 3;//entrada do buzzer
int umidadeSolo;//int que irá guardar o valor da umidade em porcentagem
int sensorSolo = A3;//entrada do sensor
int umidadeBruto;//valor bruto do sensor de umidade
int motorPin =5;//pino do motor
int motorState = 0;//estado do motor

int resistorLuz = A0; //colocando a entrada do fotoresistor
int luminosidade = 0; //colocando o valor da luminosidade

void setup()
{  
  
  lcd.begin(16, 2);//Inicia o display
  lcd.clear();//Limpa a tela
  lcd.setCursor(0, 0);//Posiciona o cursor na coluna 0, linha 0 
  Serial.begin(9600);//iniciando monitor serial
  lcd.print("Sensor de temp.");
  lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
  lcd.print("e luminosidade");
  pinMode(A1, INPUT);//configura o TMP como entrada
  pinMode(motorPin, OUTPUT);//configura o motor como entrada
}

void loop()
{
  pegarValores();//função que irá obter os valores dos sensores
  verificarTemp();//função para verificar a temperatura
  delay(1000);//espera 1 segundo
  verificarLuz();//função que irá verificar a luminosidade
  delay(1000);//espera 1 segundo
  verificarUmidade();//função para verificar a umidade do solo
  delay(1000);//espera 1 segundo
  condicoesPraga();//função que alertará o usuário, caso haja condições para praga
}

void pegarValores()
{
  motorState = digitalRead(motorPin); 
  
  //ver umidade do solo 
  umidadeBruto = analogRead(sensorSolo);//pega o valor da umidade da terra que estará entre 0 e 876
  umidadeSolo = map(umidadeBruto,0,876,0,100);//irá fazer os valores dos sensores irem de 0 a 100
  Serial.print("Umidade do solo: ");
  Serial.println(umidadeSolo);
  
  
  //ver temp
  RawValue = analogRead(tempSensor);//recebe o valor do sensor
  Voltage = (RawValue / 1023.0) * 5000;//pega o valor da voltagem do sensor
  tempC = (Voltage-500) * 0.1;//transforma a voltagem em temepratura
  Serial.print("Temperatura em C = ");
  Serial.println(tempC,1);
  delay(1000);//espera 1 segundo
}

void verificarTemp()
{
  if(tempC < 5)//irá alertar o usuário caso a temperatura esteja muito baixa
  {
    noTone(altoFalante);//removerá o som do alarme
    lcd.clear();
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0
    lcd.print("Temp. baixa");
    lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
    lcd.print(tempC);
    tone(altoFalante,1500);//tocar alarme a 1500 hertz
    arrumar_fator();
  }
  else if(tempC > 25)//irá alertar o usuário caso a temperatura comece a ficar muito alta
  {
    noTone(altoFalante);//removerá o som do alarme
    lcd.clear();
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0 
    lcd.print("Temp. alta");
    lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
    lcd.print(tempC);
    tone(altoFalante,2000);//tocar alarme a 2000 hertz
    arrumar_fator();  
  }
    else //indicará que a temperatura está normal
  {
    noTone(altoFalante);//removerá o som do alarme
    lcd.clear();
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0 
    lcd.print("Temp. OK");
    lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
    lcd.print(tempC);
  }
}

void verificarUmidade()
{
  if(umidadeSolo < 60)
  {
    noTone(altoFalante);//removerá o som do alarme
    lcd.clear();//Limpa a tela
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0 
    lcd.print("Solo seco");
    lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
    lcd.print(umidadeSolo);//mostra a porcentagem de umidade no display
    delay(1000);//espera 1 segundo
    lcd.clear();//Limpa a tela
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0 
    lcd.print("Ligando motor");
    digitalWrite(motorPin,LOW);//mostra a porcentagem de umidade no display
    tone(altoFalante,1000);//tocar alarme a 1000 hertz
    arrumar_fator();
  }
  else if(umidadeSolo > 70)
  {
    noTone(altoFalante);//removerá o som do alarme
    digitalWrite(motorPin,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0 
    lcd.print("Solo umido");
    lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
    lcd.print(umidadeSolo);//mostra a porcentagem de umidade no display
    tone(altoFalante,2500);//tocar alarme a 2500 hertz
    arrumar_fator();
  }
  else
  {
    noTone(altoFalante);//removerá o som do alarme
    digitalWrite(motorPin,HIGH);
    lcd.clear();//Limpa a tela
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0 
    lcd.print("Solo OK");
    lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
    lcd.print(umidadeSolo);//mostra a porcentagem de umidade no display
  }
}
void condicoesPraga()
{
  if(tempC > 25 && umidadeSolo > 70)
  {
    noTone(altoFalante);//removerá o som do alarme
    lcd.clear();//Limpa a tela
    lcd.setCursor(0,0);//Posiciona o cursor na coluna 0, linha 0 
    lcd.print("Aviso:");
    lcd.setCursor(0,1);//Posiciona o cursor na coluna 0, linha 1
    lcd.print("Perigo de praga");
    tone(altoFalante,3000);
    arrumar_fator();
  }
}
void arrumar_fator()//essa função irá servir para que o alarme continue tocando até que o valor do sensor esteja normal
{
  while(tempC > 25 || tempC < 5 || umidadeSolo < 60 || umidadeSolo > 70)
  {
    umidadeBruto = analogRead(sensorSolo);
    umidadeSolo = map(umidadeBruto,0,876,0,100);
    tempC = (Voltage-500) * 0.1;
    Serial.print("Umidade do solo: ");
  	Serial.println(umidadeSolo);
    Serial.print("Temperatura em C = ");
  	Serial.println(tempC,1);
  }
}
