#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define MAX_DISTANCE 120

#define TRIGGER_PIN2 11
#define ECHO_PIN2 12
#define MAX_DISTANCE2 16 // Ubah jarak maksimum menjadi 16 cm

#define SERVO_PIN A0 // Menggunakan pin A0 untuk servo

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
NewPing sonar1(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE2);

bool isTrashFull = false;

void setup() {
  myservo.attach(SERVO_PIN);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  delay(50);

  // Membaca jarak dengan ultrasonik 1
  int distance1 = sonar1.ping_cm();
  
  // Membaca jarak dengan ultrasonik 2
  int distance2 = sonar2.ping_cm();
  
  // Membuka tong sampah jika ada objek terdeteksi oleh ultrasonik 1
  if (distance1 > 0 && distance1 < 20) {
    // Menggerakkan servo perlahan ke posisi terbuka (0 derajat)
    for (int pos = myservo.read(); pos >= 0; pos--) {
      myservo.write(pos);
      delay(5); // Penundaan untuk pergerakan yang lebih lambat
    }
    delay(2000); // Memberi waktu untuk membuang sampah
    // Menggerakkan servo perlahan kembali ke posisi tengah (90 derajat)
    for (int pos = 0; pos <= 90; pos++) {
      myservo.write(pos);
      delay(5); // Penundaan untuk pergerakan yang lebih lambat
    }
  }

  // Memeriksa kapasitas sampah dan menampilkan pada LCD
  int capacityPercentage = 0;
  if (distance2 > 0 && distance2 <= 16) {
    capacityPercentage = map(distance2, 16, 0, 0, 100);
  } else if (distance2 > 16) {
    capacityPercentage = 0; // Mengatasi kasus ketika tidak ada sampah terdeteksi
  }

  lcd.setCursor(0, 0);
  lcd.print("Capacity: ");
  lcd.print(capacityPercentage);
  lcd.print("%    ");
  
  // Memeriksa jika kapasitas sampah penuh
  if (capacityPercentage >= 90) {
    isTrashFull = true;
  } else {
    isTrashFull = false;
  }

  // Menampilkan pesan pada LCD jika kapasitas penuh
  if (isTrashFull) {
    lcd.setCursor(0, 1);
    lcd.print("Trash Full!");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("            "); // Menghapus pesan sebelumnya jika kapasitas tidak penuh
  }
}
