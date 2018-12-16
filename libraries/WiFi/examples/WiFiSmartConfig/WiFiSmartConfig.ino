#include "WiFi.h"
#include "mbedtls/md.h"   //Included for generation of key

void setup() {
  Serial.begin(115200);

  //Init WiFi as Station, start SmartConfig
  WiFi.mode(WIFI_AP_STA);

// --------------------------------------------------------------------
  char *prodKey = "";                       //ProductKey = Per Vendor
  char *devID = "";                         //DeviceID   = Per IoT Device
  byte hmacResult[32];
 
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
 
  const size_t devIDLength = strlen(devID);
  const size_t prodKeyLength = strlen(prodKey);            
 
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char *) prodKey, prodKeyLength);
  mbedtls_md_hmac_update(&ctx, (const unsigned char *) devID, devIDLength);
  mbedtls_md_hmac_finish(&ctx, hmacResult);
  mbedtls_md_free(&ctx);
 
  //Serial.print("Hash: ");
  /*
  for(int i= 0; i< sizeof(hmacResult); i++){
      char str[3];
 
      sprintf(str, "%02x", (int)hmacResult[i]);
      Serial.print(str);
  }
  */

  //WiFi.beginSmartConfig(hmacResult);      //Enter param (byte hmacResult[32])
  WiFi.beginSmartConfig();
// --------------------------------------------------------------------

  //Wait for SmartConfig packet from mobile
  Serial.println("Waiting for SmartConfig.");
  while (!WiFi.smartConfigDone()) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("SmartConfig received.");

  //Wait for WiFi to connect to AP
  Serial.println("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi Connected.");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
