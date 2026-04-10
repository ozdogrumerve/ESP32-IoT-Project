#include <DHT.h>              // DHT sensörü için kütüphane
#include <Wire.h>             // I2C haberleşme
#include <LiquidCrystal_I2C.h> // LCD kütüphanesi

// ===================== PIN TANIMLARI =====================

// DHT22 pin
#define DHTPIN 15
#define DHTTYPE DHT22

// LDR pin
#define LDR_PIN 34

// RGB LED pinleri
#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27

// Buton
#define BUTTON_PIN 18

// ===================== NESNE OLUŞTURMA =====================

DHT dht(DHTPIN, DHTTYPE); // DHT sensörü nesnesi
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD nesnesi (adres: 0x27, 16x2 ekran)

// ===================== DEĞİŞKENLER =====================

unsigned long previousMillis = 0;  // LCD ekran değişim zamanını tutar
unsigned long displayTime = 2000;  // LCD'de her ekranın kalma süresi (ms)

int screenIndex = 0;               // Round-robin için ekran sırası (0=Nem, 1=Sıcaklık, 2=Işık)
unsigned long lastSerialPrint = 0; // Serial monitöre yazdırma zaman kontrolü

// PWM ayarları
const int freq = 5000; // LED'ler için PWM frekansı
const int resolution = 8; // PWM çözünürlüğü (0-255)

bool lastButtonState = HIGH; // Butonun önceki durumu (debounce için)

// ===================== SETUP =====================


void setup() {
  Serial.begin(115200);   // Seri haberleşmeyi başlat (115200 baud)
  dht.begin();            // DHT22 sensörünü başlat

  lcd.init();             // LCD'yi başlat
  lcd.backlight();        // LCD arka ışığını aç

  pinMode(LDR_PIN, INPUT); // LDR pinini giriş olarak ayarla

  pinMode(RED_PIN, OUTPUT); // Kırmızı LED pini çıkış
  pinMode(GREEN_PIN, OUTPUT); // Yeşil LED pini çıkış
  pinMode(BLUE_PIN, OUTPUT); // Mavi LED pini çıkış

  pinMode(BUTTON_PIN, INPUT_PULLUP); // Buton pinini pull-up direnci ile giriş yap

  // PWM kanallarını LED pinlerine bağla
  ledcAttach(RED_PIN, freq, resolution);
  ledcAttach(GREEN_PIN, freq, resolution);
  ledcAttach(BLUE_PIN, freq, resolution);
}


// ===================== LOOP =====================

void loop() {

  // ===================== SENSOR OKUMA =====================

  float humidity = dht.readHumidity();       // Nem değerini oku (%)
  float temperature = dht.readTemperature(); // Sıcaklık değerini oku (°C)

  int ldrValue = analogRead(LDR_PIN);        // LDR'den analog değeri oku (0-4095)

  // LDR değerini yaklaşık lux değerine dönüştür
  float lux = map(ldrValue, 4095, 0, 0, 500);

  // ===================== SERIAL OUTPUT (3 SN) =====================

  if (millis() - lastSerialPrint >= 3000) { // Her 3 saniyede bir seri porta yazdır
    lastSerialPrint = millis();

    Serial.print("Nem: ");
    Serial.print(humidity, 2);
    Serial.print("% || Sicaklik: ");
    Serial.print(temperature, 2);
    Serial.print("°C || Isik: ");
    Serial.print(lux);
    Serial.println(" lux");
  }

  // ===================== BUTON =====================

  bool currentButton = digitalRead(BUTTON_PIN); // Buton durumunu oku

  // Butona basıldığında (yükselen kenar algılama)
  if (lastButtonState == HIGH && currentButton == LOW) {
    // LCD ekran süresini 2sn ↔ 4sn arasında değiştir
    if (displayTime == 2000) displayTime = 4000;
    else displayTime = 2000;

    Serial.print("LCD suresi degistirildi: ");
    Serial.print(displayTime / 1000);
    Serial.println(" sn");

    previousMillis = 0; // LCD'yi hemen yenile
  }

  lastButtonState = currentButton; // Buton durumunu güncelle

  // ===================== LCD ROUND ROBIN =====================

  if (millis() - previousMillis >= displayTime) { // Belirlenen süre dolduysa ekranı değiştir
    previousMillis = millis();
    lcd.clear(); // LCD'yi temizle

    if (screenIndex == 0) {
      // NEM
      lcd.setCursor(6, 0);
      lcd.print("NEM");

      lcd.setCursor(15, 0);
      lcd.print(displayTime / 1000); // Mevcut süre gösterimi

      lcd.setCursor(0, 1);
      lcd.print(humidity, 2);

      if (humidity <= 60)
        lcd.print(" - LOW");
      else
        lcd.print(" - HIGH");
    }

    else if (screenIndex == 1) {
      // SICAKLIK
      lcd.setCursor(4, 0);
      lcd.print("SICAKLIK");

      lcd.setCursor(15, 0);
      lcd.print(displayTime / 1000);

      lcd.setCursor(0, 1);
      lcd.print(temperature, 2);

      if (temperature < 0)
        lcd.print("- VERY LOW");
      else if (temperature <= 30)
        lcd.print(" - NORMAL");
      else
        lcd.print(" - HIGH");
    }

    else {
      // ISIK
      lcd.setCursor(6, 0);
      lcd.print("ISIK");

      lcd.setCursor(15, 0);
      lcd.print(displayTime / 1000);

      lcd.setCursor(0, 1);

      if (lux < 10)
        lcd.print("KARANLIK");
      else
        lcd.print("AYDINLIK");
    }

    screenIndex = (screenIndex + 1) % 3; // Sonraki ekrana geç (0→1→2→0)
  }

  // ===================== RGB LED =====================

  if (lux < 10) {
    // KARANLIK → MAVİ
    ledcWrite(RED_PIN, 0); // Kırmızı LED'i kapat
    ledcWrite(GREEN_PIN, 0); // Yeşil LED'i kapat
    ledcWrite(BLUE_PIN, 255); // Mavi LED'i tam güçte yak
  } 
  else {
    // AYDINLIK

    if (temperature < 0) {
      // LOW → AÇIK MAVİ
      ledcWrite(RED_PIN, 0);
      ledcWrite(GREEN_PIN, 150); // Yeşil LED'i orta seviyede yak
      ledcWrite(BLUE_PIN, 255);
    } 
    else if (temperature <= 30) {
      // NORMAL → YEŞİL
      ledcWrite(RED_PIN, 0);
      ledcWrite(GREEN_PIN, 255);
      ledcWrite(BLUE_PIN, 0);
    } 
    else {
      // HIGH → KIRMIZI
      ledcWrite(RED_PIN, 255);
      ledcWrite(GREEN_PIN, 0);
      ledcWrite(BLUE_PIN, 0);
    }
  }
  
}