package com.example.demo.processor;

import com.example.demo.model.Person;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.batch.item.ItemProcessor;


public class PersonItemProcessor implements ItemProcessor<Person, Person> {

    private static final Logger log = LogManager.getLogger();

    @Override
    public Person process(final Person person) {
        final String firstName = person.firstName().toUpperCase();
        final String lastName = person.lastName().toUpperCase();

        final Person transformedPerson = new Person(firstName, lastName);

        log.info("Converting ({}) into ({})", person, transformedPerson);

        return transformedPerson;
    }

}