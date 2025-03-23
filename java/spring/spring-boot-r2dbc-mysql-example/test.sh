#curl -v -XPUT -H 'Content-Type: application/json; charset=utf-8' -d '{"id":1,"title":"my title","description":"my first article","published":true}' http://localhost:8080/api/tutorials/1

#curl -v -X POST -H 'Content-Type: application/json; charset=utf-8' -d '{"id":1,"title":"my title","description":"my first article","published":true}' http://localhost:8080/api/tutorials
#curl -v -X POST -H 'Content-Type: application/json; charset=utf-8' -d '{"id":12,"title":"my title 12 ","description":"my 12th article","published":1}' http://localhost:8080/api/tutorials

curl -v -X PUT -H 'Content-Type: application/json; charset=utf-8' -d '{"id":12,"title":"my title 12 ,update","description":"my 12th article","published":1}' http://localhost:8080/api/tutorials/3
