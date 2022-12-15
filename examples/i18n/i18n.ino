#include <LittleFS.h>
#include <ArduinoJson.h>
#define YAML_DISABLE_CJSON // not needed here
#include <YAMLDuino.h>
#include <i18n/i18n.hpp>

#include <vector>
static std::vector<String> i18nFiles;
const char* extension = "yml";
const char* path = "/lang";

i18n_t i18n( &LittleFS ); // attach LittleFS to i18n loader

void setup()
{
  Serial.begin(115200);
  LittleFS.begin();

  Serial.printf( "[%d] %s\n", ESP.getFreeHeap(), "Hello i18n test");

  // scan the lang folder and store filenames in an array

  File dir =  LittleFS.open( path );

  if( !dir ) {
    Serial.printf("Error, can't access filesystem, halting");
    while(1) vTaskDelay(1);
  }

  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) { // no more files
      break;
    }
    if (entry.isDirectory()) continue;

    String fileName = entry.path();
    if( fileName.endsWith(  extension ) ) {
      i18nFiles.push_back( fileName );
    }
    entry.close();
  }

  dir.close();
}


void loop()
{
  int randLang = rand()%(i18nFiles.size());

  if(! i18n.setLocale(i18nFiles[randLang].c_str()) ) {

    Serial.printf( "[%d] Error loading locale %s, halting\n", ESP.getFreeHeap(), i18nFiles[randLang].c_str());

    while(1) vTaskDelay(1);
  }

  Serial.printf( "[%d] Locale file %s loaded\n", ESP.getFreeHeap(), i18nFiles[randLang].c_str());

  Serial.printf( "[%d] %s\n", ESP.getFreeHeap(), i18n.gettext("activerecord:errors:messages:record_invalid" ) ); // "La validation a échoué : %{errors}"
  Serial.printf( "[%d] %s\n", ESP.getFreeHeap(), i18n.gettext("date:abbr_day_names:2" ) ); // "mar"
  Serial.printf( "[%d] %s\n", ESP.getFreeHeap(), i18n.gettext("time:pm" ) ); // "pm", last element
  delay( 1000 );
}
