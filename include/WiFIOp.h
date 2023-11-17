#ifndef WIFIOP_H
#define WIFIOP_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SDModel.h>
#include <WiFiMulti.h>

void wifi_connect();
void wifi_ap_server();
void wifi_server_end();
void wifi_file_recv();
#endif