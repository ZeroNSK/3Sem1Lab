package main

import (
	"bufio"
	"fmt"
	"os"
)

var gM []string

func mLoad(file string) bool {
	gM = gM[:0]
	f, err := os.Open(file)
	if err != nil {
		return false
	}
	defer f.Close()
	s := bufio.NewScanner(f)
	for s.Scan() {
		gM = append(gM, s.Text())
	}
	return true
}

func mSave(file string) bool {
	f, err := os.Create(file)
	if err != nil {
		return false
	}
	defer f.Close()
	w := bufio.NewWriter(f)
	for _, v := range gM {
		fmt.Fprintln(w, v)
	}
	_ = w.Flush()
	return true
}

func mPush(v string) { gM = append(gM, v) }

func mDel(index uint) bool {
	if int(index) >= len(gM) {
		return false
	}
	i := int(index)
	copy(gM[i:], gM[i+1:])
	gM = gM[:len(gM)-1]
	return true
}

func mGet(index uint, out *string) bool {
	if int(index) >= len(gM) {
		return false
	}
	*out = gM[index]
	return true
}

func mPrint() {
	for i, v := range gM {
		if i > 0 {
			fmt.Print(" ")
		}
		fmt.Print(v)
	}
	fmt.Println()
}
