
  ssh-keygen -t rsa -m PEM 
 // ok
 C:\Program" "Files\Java\jdk1.8.0_261\bin\java -jar  .\target\sftpclient-1.0-SNAPSHOT.jar
 
 
 // failed
  C:\Program" "Files\Java\jdk1.6.0_45\bin\java -jar  .\target\sftpclient-1.0-SNAPSHOT.jar

// download successfully when maven-compiler-plugin configuration source and target both setted to 1.6
C:\Program" "Files\Java\jdk1.6.0_45\bin\java -jar  .\target\sftpclient-1.0-SNAPSHOT.jar 192.168.79.132 22 user "C:\\Users\\lenovo\\.ssh\\pem\\id_rsa"