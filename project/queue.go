package main

import (
	"bufio"
	"fmt"
	"os"
)

type qNode struct {
	person string
	next   *qNode
}

var gQHead *qNode
var gQTail *qNode

func qClear() {
	for gQHead != nil {
		gQHead = gQHead.next
	}
	gQTail = nil
}

func qPush(v string) {
	n := &qNode{person: v}
	if gQTail != nil {
		gQTail.next = n
		gQTail = n
	} else {
		gQHead, gQTail = n, n
	}
}

func qPop(out *string) bool {
	if gQHead == nil {
		return false
	}
	n := gQHead
	if out != nil {
		*out = n.person
	}
	gQHead = n.next
	if gQHead == nil {
		gQTail = nil
	}
	return true
}

func qLoad(file string) bool {
	qClear()
	f, err := os.Open(file)
	if err != nil {
		return false
	}
	defer f.Close()
	s := bufio.NewScanner(f)
	for s.Scan() {
		qPush(s.Text())
	}
	return true
}

func qSave(file string) bool {
	f, err := os.Create(file)
	if err != nil {
		return false
	}
	defer f.Close()
	w := bufio.NewWriter(f)
	for p := gQHead; p != nil; p = p.next {
		fmt.Fprintln(w, p.person)
	}
	_ = w.Flush()
	return true
}

func qPrint() {
	first := true
	for p := gQHead; p != nil; p = p.next {
		if !first {
			fmt.Print(" ")
		}
		fmt.Print(p.person)
		first = false
	}
	fmt.Println()
}
