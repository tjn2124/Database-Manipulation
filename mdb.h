/*
 * mdb.h
 */

#ifndef _MDB_H_
#define _MDB_H_

struct MdbRec {
    char name[16];//takes input followed by null terminator
    char  msg[24];//each word is followed by a null 
};

#endif /* _MDB_H_ */