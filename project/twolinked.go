package main

import (
	"bufio"
	"fmt"
	"os"
)

type dNode struct {
	person string
	prev   *dNode
	next   *dNode
}

var gLHead *dNode
var gLTail *dNode

func lClear() {
	for gLHead != nil {
		gLHead = gLHead.next
	}
	gLTail = nil
}

func lPushBack(v string) {
	n := &dNode{person: v, prev: gLTail}
	if gLTail != nil {
		gLTail.next = n
	} else {
		gLHead = n
	}
	gLTail = n
}

func lAt(index uint) *dNode {
	p := gLHead
	for p != nil && index > 0 {
		p = p.next
		index--
	}
	return p
}

func lLoad(file string) bool {
	lClear()
	f, err := os.Open(file)
	if err != nil {
		return false
	}
	defer f.Close()
	s := bufio.NewScanner(f)
	for s.Scan() {
		lPushBack(s.Text())
	}
	return true
}

func lSave(file string) bool {
	f, err := os.Create(file)
	if err != nil {
		return false
	}
	defer f.Close()
	w := bufio.NewWriter(f)
	for p := gLHead; p != nil; p = p.next {
		fmt.Fprintln(w, p.person)
	}
	_ = w.Flush()
	return true
}

func lPush(v string) { lPushBack(v) }

func lDelValue(v string) bool {
	for p := gLHead; p != nil; p = p.next {
		if p.person == v {
			if p.prev != nil {
				p.prev.next = p.next
			} else {
				gLHead = p.next
			}
			if p.next != nil {
				p.next.prev = p.prev
			} else {
				gLTail = p.prev
			}
			return true
		}
	}
	return false
}

func lGet(index uint, out *string) bool {
	p := lAt(index)
	if p == nil {
		return false
	}
	*out = p.person
	return true
}

func lPrint() {
	first := true
	for p := gLHead; p != nil; p = p.next {
		if !first {
			fmt.Print(" ")
		}
		fmt.Print(p.person)
		first = false
	}
	fmt.Println()
}
