# ESP32 Ortam İzleme Projesi

## 📌 Proje Açıklaması

Bu projede ESP32 kullanılarak ortamın sıcaklık, nem ve ışık seviyesi ölçülmektedir. Elde edilen veriler LCD ekran üzerinde gösterilmekte ve RGB LED ile görsel olarak ifade edilmektedir.

---

## 🧰 Kullanılan Bileşenler

* ESP32 DevKit
* DHT22 (Sıcaklık & Nem sensörü)
* LDR (Işık sensörü)
* RGB LED (Ortak katot)
* 16x2 I2C LCD ekran
* Buton
* Dirençler

---

## 📚 Kullanılan Kütüphaneler

* DHT.h
* Wire.h
* LiquidCrystal_I2C.h

---

## ⚙️ Çalışma Mantığı

* Sistem sıcaklık, nem ve ışık verilerini sensörlerden okur.
* LCD ekran bu verileri **Round Robin yöntemiyle** sırayla gösterir.
* Butona basıldığında LCD güncelleme süresi:

  * 2 saniye ↔ 4 saniye olarak değişir.
* RGB LED:

  * Karanlık ortam → Mavi
  * Aydınlık + düşük sıcaklık → Açık mavi
  * Aydınlık + normal sıcaklık → Yeşil
  * Aydınlık + yüksek sıcaklık → Kırmızı

---

## ▶️ Projeyi Çalıştırma Adımları

1. Wokwi linkine giriniz.
2. Simülasyonu başlatınız.
3. Sensör değerlerini değiştirerek sistemi test ediniz.
4. Butona basarak LCD süresini değiştiriniz.

---

## 🔗 Wokwi Proje Linki

https://wokwi.com/projects/460912503681169409

---

## 📞 Support & Contact

-   📧 Email: ozdogrumerve57@gmail.com 
-   🐛 Issues: Feel free to report bugs or suggest features on [GitHub Issues](https://github.com/ozdogrumerve/Spotly/issues)
-   👤 Author: [Merve Özdoğru](https://github.com/ozdogrumerve)

---

<div align="center">

**⭐ Star this repo if you find it helpful!**

Made with ❤️ by [Merve Özdoğru](https://github.com/ozdogrumerve)

