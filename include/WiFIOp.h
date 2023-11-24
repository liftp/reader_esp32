#ifndef WIFIOP_H
#define WIFIOP_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SDModel.h>
#include <WiFiMulti.h>
#include "ButtonInterrupt.h"
#include "Common.h"

void wifi_connect();
void wifi_ap_server();
void wifi_server_end();
bool wifi_file_recv();
char* wifi_get_local_ip();
#endif