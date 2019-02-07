**Wie lädt man Daten vom ESP32?**
Ganz wichtig ist das man dabei einen **CORS-SERVER** benutzt! Im Repository ist ein passender schon oben.

Im Prinzip muss nur über ein simples fetch die daten von der Website geladen werden die der ESP32 zur Verfügung stellt.

    var contentURI= "http://localhost:8080/http://192.168.0.112/get";
    
    //zu erst wird dem CORS-SERVER die URL vom ESP32 angegeben und dieser schickt dann die Resultate
    
    let response = await fetch(contentURI);