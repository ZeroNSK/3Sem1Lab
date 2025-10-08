package main

// M: массив
func M_load(file string) bool            { return mLoad(file) }
func M_save(file string) bool            { return mSave(file) }
func M_push(v string)                    { mPush(v) }
func M_del(index uint) bool              { return mDel(index) }
func M_get(index uint, out *string) bool { return mGet(index, out) }
func M_print()                           { mPrint() }

// F: односвязный список
func F_load(file string) bool            { return fLoad(file) }
func F_save(file string) bool            { return fSave(file) }
func F_push(v string)                    { fPush(v) }
func F_del_value(v string) bool          { return fDelValue(v) }
func F_get(index uint, out *string) bool { return fGet(index, out) }
func F_print()                           { fPrint() }

// L: двусвязный список
func L_load(file string) bool            { return lLoad(file) }
func L_save(file string) bool            { return lSave(file) }
func L_push(v string)                    { lPush(v) }
func L_del_value(v string) bool          { return lDelValue(v) }
func L_get(index uint, out *string) bool { return lGet(index, out) }
func L_print()                           { lPrint() }

// Q: очередь
func Q_load(file string) bool { return qLoad(file) }
func Q_save(file string) bool { return qSave(file) }
func Q_push(v string)         { qPush(v) }
func Q_pop(out *string) bool  { return qPop(out) }
func Q_print()                { qPrint() }

// S: стек
func S_load(file string) bool { return sLoad(file) }
func S_save(file string) bool { return sSave(file) }
func S_push(v string)         { sPush(v) }
func S_pop(out *string) bool  { return sPop(out) }
func S_print()                { sPrint() }
