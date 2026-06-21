# Windmill-Pitch-System-IoT-Project
---
## 1. Project Description
This project is in collaboration with CSULB ASME's Windmill Project within its Electrical Team. Specifically, this repo will cover the important software that goes into the controls, IoT, and Data Acquisition of the windmill. The purpose is to monitor the windmill power generation and blade speeds, as an indiciation of overall health, and to control the custom pitch system. 
### Project Structure
---
#### Diagram 1 - Project overview
---
<img width="1421" height="589" alt="Image" src="https://github.com/user-attachments/assets/e3eef921-12e7-491d-bd88-0a5c2f02076d" />

#### Diagram 2 - Wiring 
---
<img width="859" height="894" alt="Image" src="https://github.com/user-attachments/assets/663551fa-2f21-4877-85e3-5fdb373f3c5c" />

### Project Management
*Project Iteration & Stages*
* *Stage 1:*
* *Stage 2:*
* *Stage 3:*
* *Stage 4:*

## 2. How to Setup the Project
### Bill of Materials
---
- [ ] *[ESP32-C3 / ESP32-S3](https://www.aliexpress.us/item/3256808865544909.html?src=google&snps=y&src=google&albch=shopping&acnt=708-803-3821&isdl=y&slnk=&plac=&mtctp=&albbt=Google_7_shopping&aff_platform=google&aff_short_key=UneMJZVf&gclsrc=aw.ds&albagn=888888&ds_e_adid=&ds_e_matchtype=&ds_e_device=c&ds_e_network=x&ds_e_product_group_id=&ds_e_product_id=en3256808865544909&ds_e_product_merchant_id=109165979&ds_e_product_country=US&ds_e_product_language=en&ds_e_product_channel=online&ds_e_product_store_id=&ds_url_v=2&albcp=20542171667&albag=&isSmbAutoCall=false&needSmbHouyi=false&gad_source=1&gad_campaignid=18545443176&gbraid=0AAAAAD6I-hGOXEvgiHp-2TBHltPVyDbvF&gclid=CjwKCAjwl97RBhBWEiwAa9rbXY3NgC_GOsOra9a9jU6myaY7XlM0zseo2D-FGgvTqacPTZ4Xfa5ZWhoCqBgQAvD_BwE&gatewayAdapt=glo2usa)* 
- [ ] *[Daryl's Servo Motor](https://www.servocity.com/daryl-nylon-gears-180-rotation-0-15s-60-53-oz-in/)* 
- [ ] *[Current Sensor](https://www.aliexpress.us/item/3256807088246731.html?_randl_currency=USD&src=google&snps=y&src=google&albch=shopping&acnt=708-803-3821&isdl=y&slnk=&plac=&mtctp=&albbt=Google_7_shopping&aff_platform=google&aff_short_key=UneMJZVf&gclsrc=aw.ds&albagn=888888&ds_e_adid=&ds_e_matchtype=&ds_e_device=c&ds_e_network=x&ds_e_product_group_id=&ds_e_product_id=en3256807088246731&ds_e_product_merchant_id=5293214119&ds_e_product_country=US&ds_e_product_language=en&ds_e_product_channel=online&ds_e_product_store_id=&ds_url_v=2&albcp=20542171667&albag=&isSmbAutoCall=false&needSmbHouyi=false&gad_source=1&gad_campaignid=18545443176&gbraid=0AAAAAD6I-hGOXEvgiHp-2TBHltPVyDbvF&gclid=CjwKCAjwl97RBhBWEiwAa9rbXSmRZqrELuIcXz33YVCdGTuO8bEBCO3pItj4MwAxBDNgiFusgv7RQBoC3HIQAvD_BwE&gatewayAdapt=glo2usa&_randl_shipto=US)*
- [ ] *[Hall Effect Sensor](https://www.aliexpress.us/item/3256806209973509.html?src=google&snps=y&src=google&albch=shopping&acnt=708-803-3821&isdl=y&slnk=&plac=&mtctp=&albbt=Google_7_shopping&aff_platform=google&aff_short_key=UneMJZVf&gclsrc=aw.ds&albagn=888888&ds_e_adid=&ds_e_matchtype=&ds_e_device=c&ds_e_network=x&ds_e_product_group_id=&ds_e_product_id=en3256806209973509&ds_e_product_merchant_id=5293214119&ds_e_product_country=US&ds_e_product_language=en&ds_e_product_channel=online&ds_e_product_store_id=&ds_url_v=2&albcp=20542171667&albag=&isSmbAutoCall=false&needSmbHouyi=false&gad_source=1&gad_campaignid=18545443176&gbraid=0AAAAAD6I-hGOXEvgiHp-2TBHltPVyDbvF&gclid=CjwKCAjwl97RBhBWEiwAa9rbXR19VDLewRIkgi7f4Qspq-gWDZ5OofSjqOa3GA1UugKIxgfCeM77LhoC9KIQAvD_BwE&gatewayAdapt=glo2usa)*
- [ ] *[Anemometer](https://www.sparkfun.com/anemometer.html#content-documentation)*
- [ ] *[Basics Electronic Components Kit](https://www.aliexpress.us/item/3256807117187614.html?_randl_currency=USD&src=google&snps=y&src=google&albch=shopping&acnt=708-803-3821&isdl=y&slnk=&plac=&mtctp=&albbt=Google_7_shopping&aff_platform=google&aff_short_key=UneMJZVf&gclsrc=aw.ds&albagn=888888&ds_e_adid=&ds_e_matchtype=&ds_e_device=c&ds_e_network=x&ds_e_product_group_id=&ds_e_product_id=en3256807117187614&ds_e_product_merchant_id=5519321870&ds_e_product_country=US&ds_e_product_language=en&ds_e_product_channel=online&ds_e_product_store_id=&ds_url_v=2&albcp=20542171667&albag=&isSmbAutoCall=false&needSmbHouyi=false&gad_source=1&gad_campaignid=18545443176&gbraid=0AAAAAD6I-hGOXEvgiHp-2TBHltPVyDbvF&gclid=CjwKCAjwl97RBhBWEiwAa9rbXVCMqMQJgJAtTffZ8R6Vy83EnvvHA9JrJjQOuTYUbpQhwkFI1QukhBoCVpQQAvD_BwE&gatewayAdapt=glo2usa&_randl_shipto=US)*
### Overview 
---
* *Software Tools:*
    * Arduino IDE
    * VS Code + PlatformIO
* *Necessary Libraries*
    * `Adafruit_NeoPixel.h`: for debuggign & LED testing using ESP32's on-board RGB LED.
    * `ESP32Servo.h`: for servo control using PWM.
    * `AsyncTCP.h & ESPAsyncWebServer.h`: for ESP32 hosted webpage for sensor reading & data acquisition.
### `platformio.ini`
---
```bash
[env:freenove_esp32_s3_wroom]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
lib_deps = 
	adafruit/Adafruit INA219@^1.2.3
	adafruit/Adafruit NeoPixel@^1.15.5
	madhephaestus/ESP32Servo@^3.2.0
	esp32async/AsyncTCP@^3.4.10
	me-no-dev/ESPAsyncWebServer@^3.6.0
monitor_speed = 115200
```
### Step-by-Step Guide
---
*NOTE: This project was developed in VS Code using the extension Platform I/O with associated extensions. Thus, this guide will showcase a guide using the below tools & libraries.*
1. *[Download VS Code](https://code.visualstudio.com/download?_exp_download=fb315fc982)*
2. *[Download Platformio VS Code Extension]()*
3. *[Create Platformio Project](https://platformio.org/install/ide?install=vscode)*
    * *For More:* [Link](https://docs.platformio.org/en/latest/integration/ide/vscode.html)
4. *Copy & Paste Code from This Repo or Git Clone*
5. *Add necessary libraries. See **Section Overview***

