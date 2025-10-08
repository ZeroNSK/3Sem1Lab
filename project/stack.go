package main

import (
	"bufio"
	"fmt"
	"os"
)

type sNode struct {
	person string
	next   *sNode
}

var gSTop *sNode

func sClear() {
	for gSTop != nil {
		gSTop = gSTop.next
	}
}

func sPush(v string) { gSTop = &sNode{person: v, next: gSTop} }

func sPop(out *string) bool {
	if gSTop == nil {
		return false
	}
	n := gSTop
	if out != nil {
		*out = n.person
	}
	gSTop = n.next
	return true
}

func sLoad(file string) bool {
	sClear()
	f, err := os.Open(file)
	if err != nil {
		return false
	}
	defer f.Close()
	s := bufio.NewScanner(f)
	for s.Scan() {
		sPush(s.Text())
	} // низ->верх, как в исходнике
	return true
}

func sSave(file string) bool {
	// сохранить низ->верх: соберём список и выведем в обратном порядке
	var arr []string
	for p := gSTop; p != nil; p = p.next {
		arr = append(arr, p.person)
	}
	f, err := os.Create(file)
	if err != nil {
		return false
	}
	defer f.Close()
	w := bufio.NewWriter(f)
	for i := len(arr) - 1; i >= 0; i-- {
		fmt.Fprintln(w, arr[i])
	}
	_ = w.Flush()
	return true
}

func sPrint() {
	first := true
	for p := gSTop; p != nil; p = p.next {
		if !first {
			fmt.Print(" ")
		}
		fmt.Print(p.person)
		first = false
	}
	fmt.Println()
}
