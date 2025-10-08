package main

import (
	"bufio"
	"fmt"
	"os"
)

type fNode struct {
	person string
	next   *fNode
}

var gFHead *fNode
var gFTail *fNode

func fClear() {
	for gFHead != nil {
		gFHead = gFHead.next
	}
	gFTail = nil
}

func fPushBack(v string) {
	n := &fNode{person: v}
	if gFTail == nil {
		gFHead, gFTail = n, n
	} else {
		gFTail.next = n
		gFTail = n
	}
}

func fAt(index uint) *fNode {
	p := gFHead
	for p != nil && index > 0 {
		p = p.next
		index--
	}
	return p
}

func fLoad(file string) bool {
	fClear()
	f, err := os.Open(file)
	if err != nil {
		return false
	}
	defer f.Close()
	s := bufio.NewScanner(f)
	for s.Scan() {
		fPushBack(s.Text())
	}
	return true
}

func fSave(file string) bool {
	f, err := os.Create(file)
	if err != nil {
		return false
	}
	defer f.Close()
	w := bufio.NewWriter(f)
	for p := gFHead; p != nil; p = p.next {
		fmt.Fprintln(w, p.person)
	}
	_ = w.Flush()
	return true
}

func fPush(v string) { fPushBack(v) }

func fDelValue(v string) bool {
	if gFHead == nil {
		return false
	}
	if gFHead.person == v {
		gFHead = gFHead.next
		if gFHead == nil {
			gFTail = nil
		}
		return true
	}
	prev := gFHead
	for prev.next != nil && prev.next.person != v {
		prev = prev.next
	}
	if prev.next == nil {
		return false
	}
	if prev.next == gFTail {
		gFTail = prev
	}
	prev.next = prev.next.next
	return true
}

func fGet(index uint, out *string) bool {
	p := fAt(index)
	if p == nil {
		return false
	}
	*out = p.person
	return true
}

func fPrint() {
	first := true
	for p := gFHead; p != nil; p = p.next {
		if !first {
			fmt.Print(" ")
		}
		fmt.Print(p.person)
		first = false
	}
	fmt.Println()
}
