module example/hello

go 1.17

require example.com/greetings v0.0.0-00010101000000-000000000000

require (
	github.com/go-sql-driver/mysql v1.6.0 // indirect
	github.com/jmoiron/sqlx v1.3.4 // indirect
)

replace example.com/greetings => ../greetings
