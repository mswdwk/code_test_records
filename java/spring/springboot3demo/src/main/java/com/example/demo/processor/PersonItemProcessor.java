package com.example.demo.processor;

import com.example.demo.model.Person2;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.batch.item.ItemProcessor;


public class PersonItemProcessor implements ItemProcessor<Person2, Person2> {

    private static final Logger log = LogManager.getLogger();

    @Override
    public Person2 process(final Person2 person) {
        final String firstName = person.getFirstName().toUpperCase();
        final String lastName = person.getLastName().toUpperCase();

        final Person2 transformedPerson = new Person2();
        transformedPerson.setFirstName(firstName);
        transformedPerson.setLastName(lastName);

        log.info("Converting ({}) into ({})", person, transformedPerson);

        return transformedPerson;
    }

}