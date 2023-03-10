use sqlparser::ast::Statement;
use sqlparser::dialect::GenericDialect;
use sqlparser::parser::Parser;
use std::env::args;
use std::fs::File;
use std::hash::Hash;
use std::io::{BufRead, BufReader};
use std::time;
use std::time::{Duration, Instant};

fn parse_sql_test1() {
    let dialect = GenericDialect {}; // or AnsiDialect

    let sql = "SELECT a, b, 123, myfunc(b) \
           FROM table_1 \
           WHERE a > b AND b < 100 \
           ORDER BY a DESC, b";
    let now = Instant::now();
    let i = 0;
    let sql_count: u64 = 10000;
    let mut ast;
    let mut total_sql_length: u64 = 0;
    for i in 0..sql_count {
        ast = Parser::parse_sql(&dialect, sql).unwrap();
        total_sql_length += sql.len() as u64;
    }
    let cost_ms = now.elapsed().as_millis();
    let avg_sql_cost = now.elapsed().as_micros() as u64 / sql_count;
    let avg_sql_length = total_sql_length / sql_count;
    println!(
        "Parse sql {} times, cost time {} ms.\nAvg {}  us/sql, avg sql length {}",
        sql_count, cost_ms, avg_sql_cost, avg_sql_length
    );
    // let ast1 : Statement = ast[0];
    // println!("AST: {:?}", ast1);
    // println!("ast2 {:?}",ast1.)
}

fn help() {
    println!("Usage: ./program input_sql_file");
}

fn main() {
    let mut args = args();
    if args.len() != 2 {
        help();
        return;
    }
    let dialect = GenericDialect {}; // or AnsiDialect
    let _ = args.next();
    let input_sql_file = args.next().unwrap_or("input.sql".to_string());
    println!("input sql file name: {}", input_sql_file);
    let input: File = File::open(input_sql_file).unwrap();
    let buffered: BufReader<File> = BufReader::new(input);
    let mut sql_count = 0;
    let mut ignore_sql_count = 0;
    // let mut ast;
    let mut error_sql_count = 0;
    let mut total_sql_length: u64 = 0;
    let now = Instant::now();
    for line in buffered.lines().map(|x| x.unwrap()) {
        // line: String     x:Result<String, Error>
        // println!("{}", line);
        if line.len() < 3 || line.starts_with("--"){
            ignore_sql_count += 1;
            continue;
        }

        if Parser::parse_sql(&dialect, &line).is_err() {
            error_sql_count += 1;
        }
        total_sql_length += line.len() as u64;
        sql_count += 1;
    }
    let cost_ms = now.elapsed().as_millis();
    let avg_sql_cost = now.elapsed().as_micros() as u64 / sql_count;
    let avg_sql_length = total_sql_length / sql_count;
    println!("Parse sql {} times, ignore_sql_count {} error_sql_count {} cost time {} ms.\n\
             Avg {} us/sql, avg sql length {}",
             sql_count,ignore_sql_count,error_sql_count,cost_ms,avg_sql_cost,avg_sql_length);
}
