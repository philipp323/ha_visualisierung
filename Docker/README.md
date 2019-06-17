## Wie kann man unser Projekt dockern?

1.) Unser GitHub-Repository clonen  
2.) Mit Docker Terminal in das lokal geklonte Repo wechseln  
3.) `docker build -t app .` eingeben und Enter drücken  
4.) Port von Docker-Maschine auf 80 leiten
z.B.: `docker run -p 9000:80 app`   
5.) Im Browser dann `IP-Adresse-Docker:9000` 
z.B.: http://192.168.99.100:9000/
