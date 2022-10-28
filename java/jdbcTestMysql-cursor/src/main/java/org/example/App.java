package org.example;

public class App 
{
    public static void main( String[] args ) {
        Dbconnect dbconnect = new Dbconnect();
        dbconnect.flow_search();
        dbconnect.cursor_search();
    }
}
