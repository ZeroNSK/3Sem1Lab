package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func splitCmd(line string) (cmd, arg string) {
	p := strings.IndexByte(line, ' ')
	if p == -1 {
		return line, ""
	}
	return line[:p], strings.TrimSpace(line[p+1:])
}

func main() {
	// Загрузка всех структур при старте (аналог C++ main) — см. ds_bind.h
	_ = M_load("M.data")
	_ = F_load("F.data")
	_ = L_load("L.data")
	_ = Q_load("Q.data")
	_ = S_load("S.data")

	s := bufio.NewScanner(os.Stdin)
	s.Buffer(make([]byte, 0, 64*1024), 10*1024*1024)
	for s.Scan() {
		line := strings.TrimSpace(s.Text())
		if line == "" {
			continue
		}
		cmd, arg := splitCmd(line)

		switch cmd {
		// M — массив
		case "MPUSH":
			M_push(arg)
			_ = M_save("M.data")
		case "MDEL":
			if i, err := strconv.ParseUint(arg, 10, 64); err == nil {
				if !M_del(uint(i)) {
					fmt.Println("ERR")
				} else {
					_ = M_save("M.data")
				}
			} else {
				fmt.Println("ERR")
			}
		case "MGET":
			if i, err := strconv.ParseUint(arg, 10, 64); err == nil {
				var out string
				if M_get(uint(i), &out) {
					fmt.Println(out)
				} else {
					fmt.Println("null")
				}
			} else {
				fmt.Println("null")
			}

		// F — односвязный
		case "FPUSH":
			F_push(arg)
			_ = F_save("F.data")
		case "FDEL":
			if !F_del_value(arg) {
				fmt.Println("not found")
			} else {
				_ = F_save("F.data")
			}
		case "FGET":
			if i, err := strconv.ParseUint(arg, 10, 64); err == nil {
				var out string
				if F_get(uint(i), &out) {
					fmt.Println(out)
				} else {
					fmt.Println("null")
				}
			} else {
				fmt.Println("null")
			}

		// L — двусвязный
		case "LPUSH":
			L_push(arg)
			_ = L_save("L.data")
		case "LDEL":
			if !L_del_value(arg) {
				fmt.Println("not found")
			} else {
				_ = L_save("L.data")
			}
		case "LGET":
			if i, err := strconv.ParseUint(arg, 10, 64); err == nil {
				var out string
				if L_get(uint(i), &out) {
					fmt.Println(out)
				} else {
					fmt.Println("null")
				}
			} else {
				fmt.Println("null")
			}

		// Q — очередь
		case "QPUSH":
			Q_push(arg)
			_ = Q_save("Q.data")
		case "QPOP":
			var out string
			if Q_pop(&out) {
				fmt.Println(out)
				_ = Q_save("Q.data")
			} else {
				fmt.Println("null")
			}

		// S — стек
		case "SPUSH":
			S_push(arg)
			_ = S_save("S.data")
		case "SPOP":
			var out string
			if S_pop(&out) {
				fmt.Println(out)
				_ = S_save("S.data")
			} else {
				fmt.Println("null")
			}

		case "PRINT":
			switch strings.ToLower(arg) {
			case "m":
				M_print()
			case "f":
				F_print()
			case "l":
				L_print()
			case "q":
				Q_print()
			case "s":
				S_print()
			default:
				fmt.Println("ERR")
			}
		default:
			fmt.Println("ERR")
		}
	}

	_ = M_save("M.data")
	_ = F_save("F.data")
	_ = L_save("L.data")
	_ = Q_save("Q.data")
	_ = S_save("S.data")
}
