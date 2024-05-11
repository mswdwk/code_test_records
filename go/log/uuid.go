// http://www.manongjc.com/detail/56-yhifdarhdpwfeld.html
package main

import (
	"log"

	"github.com/gofrs/uuid"
)

// Create a Version 4 UUID, panicking on error.
// Use this form to initialize package-level variables.
var _ = uuid.Must(uuid.NewV4())

func test_uuid_v4() {
	// Create a Version 4 UUID.
	u2, err := uuid.NewV4()
	if err != nil {
		log.Fatalf("failed to generate UUID: %v", err)
	}
	log.Printf("generated Version 4 UUID %v", u2)

	// Parse a UUID from a string.
	s := "6ba7b810-9dad-11d1-80b4-00c04fd430c8"
	u3, err := uuid.FromString(s)
	if err != nil {
		log.Fatalf("failed to parse UUID %q: %v", s, err)
	}
	log.Printf("successfully parsed UUID %v", u3)
}

func test_uuid_v1() {
	// Create a Version 4 UUID.
	u2, err := uuid.NewV1()
	if err != nil {
		log.Fatalf("failed to generate UUID: %v", err)
	}
	log.Printf("generated Version 1 UUID %v", u2)

	// Parse a UUID from a string.
	s := "6ba7b810-9dad-11d1-80b4-00c04fd430c8"
	u3, err := uuid.FromString(u2.String())
	if err != nil {
		log.Fatalf("failed to parse UUID %q: %v", s, err)
	}
	log.Printf("successfully parsed UUID %v", u3)
}

func tmap() *map[string]int {
	log.Printf("return map ")
	a := make(map[string]int)
	a["key"] = 123
	return &a
}

func test_map1() {
	a := tmap()
	var k = "key"
	if nil != a {
		b := *a
		if v, ok := b[k]; ok {
			log.Printf("v= %v\n", v)
		} else {
			log.Printf("not found key: %s\n", k)
		}
	}
}

func tmap2() *map[string]*int {
	log.Printf("return map 2")
	m := make(map[string]*int)
	var b = 1234567890
	m["key"] = &b
	return &m
}

func test_map2(k string) {
	a := tmap2()
	if nil != a {
		m := *a
		if v, ok := m[k]; ok {
			log.Printf("v= %v\n", *v)
		} else {
			log.Printf("not found key: %s\n", k)
		}
	}
}

func main() {
	test_uuid_v1()
	test_uuid_v4()
	test_map1()
	test_map2("key")
	test_map2("key2")
}
