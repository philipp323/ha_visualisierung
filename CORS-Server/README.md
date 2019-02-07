
**Wieso braucht man einen CORS-SERVER?**

  

**CORS** (Cross-Origin Resource Sharing) ist ein Mechanismus, der es erlaubt Ressourcen von anderen Servern zu laden, als dem, der die Webseite initial ausgeliefert hat. Das wird normalerweise von Browsern auf Grund der [same-origin security policy](https://developer.mozilla.org/en-US/docs/Web/Security/Same-origin_policy) verhindert.

  

Mit gewissen [HTTP Headern](https://developer.mozilla.org/de/docs/Glossary/Header  "Die Definition dieses Ausdrucks (HTTP Headern) wurde noch nicht geschrieben; bitte hilf mit und trage sie bei!") können Webserver jedoch die eigenen Ressourcen für den Zugriff von anderen Quellen freigeben. Durch die Definition von CORS kann einem Client mitgeteilt werden, welche Cross Origin Requests erlaubt werden sollen. Somit kann auf sichere Art und Weise die SOP umgangen werden.

  

**Was ist die Same-origin-policy?**

Die Same Origin Policy ist eine Funktion, die in den meisten modernen Browsern implementiert ist, um das Laden von Ressourcen (z. B. Bilder, Schriftarten, Skripte, …) durch Skriptsprachen (z. B. JavaScript, ActionScript) auf Clientseite nur von der eigenen Quelle (Origin) zu erlauben. Somit wird effektiv verhindert, dass Objekte durch clientseitige Skripte von fremden Webseiten geladen werden. Die Quelle (Origin) einer Webseite wird durch das Protokoll (z. B. HTTPS), die Domäne (z. B. www.google.at) und den Port (z. B. 443) definiert.


**Wie startet man diesen Server in VS Code?**   
1.) Terminal öffnen   
2.) Eingabe: `npm index.js` 

