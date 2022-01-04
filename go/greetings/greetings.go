package greetings

import (
	"errors"
	"fmt"
	"io/ioutil"
	"log"
)

// Hello returns a greeting for the named person.
func Hello(name string) (string, error) {
	if name == "" {
		return "", errors.New("Empty Name!")
	}
	// Return a greeting that embeds the name in a message.
	message := fmt.Sprintf("Hi, %v. Welcome!", name)
	return message, nil
}

func ListFiles(dirname string, level int) {
	// level用来记录当前递归的层次
	// 生成有层次感的空格
	s := "|--"
	for i := 0; i < level; i++ {
		s = "|   " + s
	}

	fileInfos, err := ioutil.ReadDir(dirname)
	if err != nil {
		log.Fatal(err)
	}
	for _, fi := range fileInfos {
		if fi.Name() == ".git" {
			continue
		}
		filename := dirname + "/" + fi.Name()
		fmt.Printf("%s%s\n", s, fi.Name())
		if fi.IsDir() {
			//继续遍历fi这个目录
			ListFiles(filename, level+1)
		}
	}
}
