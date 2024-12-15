// Nama Program : date.h
// Deskripsi    : Deklarasi fungsi date untuk membuat tanggal dan waktu
// Pembuat      : Ikhsan Satriadi / 241511080
//                Rizky Satria Gunawan / 241511089

#ifndef DATE_H
#define DATE_H

#include <stdbool.h>

// Tipe data DateTime untuk tanggal dan waktu
typedef struct
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} DateTime;

// Fungsi untuk membuat tanggal dan waktu
// Parameter : year, month, day, hour, minutes, second, bertipe integer
// Mengembalikan objek bertipe DateTime
DateTime create_date(int year, int month, int day, int hour, int minute, int second);

// Memeriksa apakah suatu tanggal valid, yaitu dengan
// memperhatikan rentang tahun dan bulan serta
// memperhatikan batas akhir per bulan
// Parameter : date bertipe DateTime
// Mengembalikan nilai boolean
bool is_valid(DateTime date);

// Memberikan tanggal terakhir dari sebuah bulan
// Parameter : date, bertipe DateTime
// Mengembalikan integer, yaitu tanggal terakhir dari bulan
int last_day_of_month(DateTime date);

// Memeriksa apakah suatu tanggal adalah kabisat; Dipakai untuk bulan februari saja
// kabisat adalah tahun yang habis dibagi 4 kecuali jika tahun habis dibagi 100
// jika tahun habis dibagi 400 maka tahun tersebut adalah kabisat
// Parameter : date, bertipe DateTime
// Mengembalikan boolean
bool is_leap(DateTime date);

// Fungsi untuk mendapatkan tanggal dan waktu saat ini
// Mengembalikan objek bertipe DateTime
DateTime get_current_date();

// Fungsi untuk mendapatkan nama bulan singkat (Jan, Feb, Mar, ..., Des)
// date: parameter input bertipe DateTime
// Mengembalikan array char
char *get_short_month_name(DateTime date);

// Prosedur untuk mendapatkan string tanggal dan waktu dengan
// "DD MMM YYYY HH:MM:SS" cth. "16 Nov 2024 15:00:00"
// date_str: parameter output passing by reference bertipe char* (array karakter)
// date: parameter input bertipe DateTime
void date_to_string(char *date_str, DateTime date);

#endif // DATE_H