// WifiHttpServer.h
#pragma once

#include <Arduino.h>
#include <WiFiManager.h>
#include <WebServer.h>

/*
 * Der HttpServer verarbeitet einlangende HTTP-Get-Requests.
 * Neben den Standardrouten kann jedes Modul seine eigenen
 * RequestHandler registrieren und wird dann vom HttpServer
 * verständigt, wenn ein Request einlangt.
 */
class HttpServerClass : public WebServer
{
 public:
	HttpServerClass(int port = 80);
	void init();
 private:
	WiFiManager _wifiManager;
};

extern HttpServerClass HttpServer;

