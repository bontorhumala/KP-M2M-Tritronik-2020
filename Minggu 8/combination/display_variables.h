#include <TJpg_Decoder.h>
#include "SPI.h"
#include <TFT_eSPI.h>

#define FS_NO_GLOBALS
#include <FS.h>
#ifdef ESP32  // ESP32 only
#include "SPIFFS.h"
#endif

SPIClass spiSD(HSPI);
#define SD_CS 15

TFT_eSPI tft = TFT_eSPI();

// Variables
const int PIN[4] = {2,3,7,9};
const String WASHER = "Washer";
const String DRYER  = "Dryer";
String cursor_main_menu = WASHER;
char cursor_payment = 'O'; // O = ovo, Q = qris
enum setting_t {
  NETWORK,
  SETUP_WIFI,
  ABOUT,
  REBOOT
};
setting_t cursor_setting = NETWORK;
bool isMainMenuActive = false;
bool isSettingMenu = false;
//const int washing_time = 2400; // 40 minutes = 2400 seconds
const int washing_time = 120; // 2 minutes
//const int drying_time = 1800; // 30 minutes = 1800 seconds
const int drying_time = 120; // 2 minutes
unsigned long time_begin = 0;

// RESOURCES
const String LOGO_CHECKLIST = "/img_jpg_rotated/logo/checklist.jpg";
const String LOGO_IOTERA    = "/img_jpg_rotated/logo/iotera.jpg";
const String LOGO_MAIN      = "/img_jpg_rotated/logo/mainlogo.jpg";
const String LOGO_MAINBLUR  = "/img_jpg_rotated/logo/mainlogo-blur.jpg";

const String MENU_CHECKLIST         = "/img_jpg_rotated/menu/checklist.jpg";
const String MENU_DRYER_ACTIVE      = "/img_jpg_rotated/menu/icon_laundry_active.jpg";
const String MENU_DRYER_INACTIVE    = "/img_jpg_rotated/menu/icon_laundry_inactive.jpg";
const String MENU_WASHER_ACTIVE     = "/img_jpg_rotated/menu/icon_washing_active.jpg";
const String MENU_WASHER_INACTIVE   = "/img_jpg_rotated/menu/icon_washing_inactive.jpg";
const String MENU_REBOOT            = "/img_jpg_rotated/menu/reboot_prompt.jpg";
const String MENU_SETTING           = "/img_jpg_rotated/menu/setting_menu.jpg" ;
const String MENU_WIFI_CONNECTED    = "/img_jpg_rotated/menu/wifi_connected.jpg";
const String MENU_WIFI_DISCONNECTED = "/img_jpg_rotated/menu/wifi_disconnected.jpg";

const String PAYMENT_LOGO_OVO   = "/img_jpg_rotated/payment/logoovo.jpg";
const String PAYMENT_LOGO_QRIS  = "/img_jpg_rotated/payment/logoqris.jpg";
const String PAYMENT_QR_OVO     = "/img_jpg_rotated/payment/ovo_qr.jpg";  // size ovo qr harus dikecilin

const String POPUP_PAYMENT_CANCEL                 = "/img_jpg_rotated/popup/cancel_qr.jpg";
const String POPUP_PAYMENT_SUCCESS                = "/img_jpg_rotated/popup/payment_success.jpg";
const String POPUP_PAYMENT_ERROR_CHECK_QR         = "/img_jpg_rotated/popup/err_check_qr.jpg";
const String POPUP_PAYMENT_ERROR_GENERATE_QR      = "/img_jpg_rotated/popup/err_generate_qr.jpg";
const String POPUP_PAYMENT_ERROR_GENERATE_INVOICE = "/img_jpg_rotated/popup/err_generate_invoice.jpg";
const String POPUP_PAYMENT_TIMEOUT                = "/img_jpg_rotated/popup/timeout_qr.jpg";
const String POPUP_PAYMENT_OPTION                 = "/img_jpg_rotated/popup/payment_option.jpg";
const String POPUP_CONFIG_RECEIVED                = "/img_jpg_rotated/popup/config_received.jpg";
const String POPUP_DISPLAYING_QR                  = "/img_jpg_rotated/popup/display_qr.jpg";
const String POPUP_ENTER_MANUAL_MODE              = "/img_jpg_rotated/popup/enter_manual_mode.jpg";
const String POPUP_FINISH_PROMPT                  = "/img_jpg_rotated/popup/finish_process.jpg";
const String POPUP_REBOOTING                      = "/img_jpg_rotated/popup/reboot.jpg";
const String POPUP_SMART_CONFIG_MODE              = "/img_jpg_rotated/popup/smart_config.jpg";
const String POPUP_CORRECT_PIN                    = "/img_jpg_rotated/popup/correct_passcode.jpg";
const String POPUP_WRONG_PIN                      = "/img_jpg_rotated/popup/wrong_passcode.jpg";

const String SETTING_ABOUT   = "/img_jpg_rotated/setting/about.jpg";
const String SETTING_NETWORK = "/img_jpg_rotated/setting/network.jpg";
const String SETTING_REBOOT   = "/img_jpg_rotated/setting/reset.jpg";
const String SETTING_SETUP   = "/img_jpg_rotated/setting/smart_config.jpg";

const String OTHERS_INVOICE = "/img_jpg_rotated/invoice.jpg";
const String OTHERS_OVO_QR  = "/img_jpg_rotated/ovo_qr.jpg";  // size ovo qr harus dikecilin

// FUNCTIONS
void DisplaySetup();
void DisplayMainMenu(button_t button);
void DisplayIntro();
bool DisplayPayment();
bool DisplayManual();
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
void textCursorChoose(String activity);
void textCursorStatus(String activity, int timeLeft);
void textLaundryDone(String activity);
void textNumberPin(int digitPlace, int number);
