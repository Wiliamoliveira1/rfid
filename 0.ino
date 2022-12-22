#include <SPI.h>
#include <MFRC522.h>

// Pino para colocar no modo "power-down"
// Não estamos utilizando no circuito proposto
#define PINO_RST  4
// Pino SS (no módulo está escrito SDA neste pino)
#define PINO_SS   2

// UID da tag responsável por liberar o acesso
int uid_tag_desejada[4] = { 0x09, 0x4A, 0x7C, 0xD5};

// Instância do módulo
MFRC522 mfrc522(PINO_SS, PINO_RST);

void setup()
{
  // Inicia a comunicação serial (monitor serial)
  Serial.begin(115200);

  // Inicia a comunicação SPI
  SPI.begin();

  // Inicia o módulo
  mfrc522.PCD_Init();

  Serial.println("Aguardando tag certa para abrir a porta...");
}

void loop()
{
  // Verifica se existe um cartão presente para leitura
  if (mfrc522.PICC_IsNewCardPresent())
  {
    // Se sim, começa a ler o cartão
    if (mfrc522.PICC_ReadCardSerial())
    {
      uint8_t liberado = 1;
     
      // Verifica os 4 bytes da UID
      Serial.print("Tag identificada: ");
      for (byte i = 0; i < 4; i++)
      {
        // Printa o byte atual
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);

        // Se o byte for diferente da tag esperada, não libera o acesso
        if(uid_tag_desejada[i] != mfrc522.uid.uidByte[i])
        {
          liberado = 0;
        }
      }
      Serial.println("");

      if(liberado)
      {
        Serial.println("Acesso liberado!");
        // Executa outras ações, como abrir uma porta
      }
      else
      {
        Serial.println("Acesso negado!");
      }
    }
   
    // Delay para não ficar lendo rapidamente
    delay(1000);
  }
}
