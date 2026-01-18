# Sudoku – OOP Projekt

## Opis

Ovo je Sudoku igra napravljena u C++ koristeći objektno-orijentirani pristup i SFML biblioteku.

## Pokretanje projekta

### Opcija 1 – Visual Studio

1. Otvoriti `Sudoku.sln`
2. Odabrati konfiguraciju **Release | x64**
3. Pokrenuti projekt (Run)

### Opcija 2 – Izvršna datoteka

* Pokrenuti `Sudoku.exe` iz Moodle ZIP predaje

## Kontrole u igri

* Klik mišem – odabir polja
* Brojevi 1–9 – unos broja
* ESC – izlaz iz igre

## Ovisnosti

* C++
* Visual Studio 2022
* SFML

## Struktura projekta

* `Sudoku/` – izvorni kod (.cpp i .h)
* `Sudoku.sln` – Visual Studio solution
* Fontovi i `.txt` datoteke koriste se kao asseti u igri

## Mogući problemi

* Ako se font ne učitava, provjeriti da je `.ttf` datoteka u istom direktoriju kao `.exe`
* Ako se projekt ne builda, provjeriti da je SFML ispravno povezan
