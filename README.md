**Dokumentacija**

Završni projekt

**Alarm sustav**

<span id="anchor"></span>

<span id="anchor-1"></span>

<span id="anchor-2"></span>1. UVOD

U ovom dokumentu dokumentiran je završni projekt iz kolegija Ugradbeni računalni sustavi. Članovi tima su Marin Car, Dino Drokan i Marin Frankić, a mentori su izv. prof. dr. sc. Mladen Tomić i asistent Arian Skoki.

U nastavku su opisane komponente, problem i rješenje projektnog zadatka.

<span id="anchor-3"></span>2 ANALIZA PROBLEMA

<span id="anchor-4"></span>2.1 Opis zadatka

Izabrali smo tri dodatne komponente: LM393 detektor zvuka, Active Buzzer Module i Matrix tipkalo.

Nakon što smo izabrali komponente morali smo smisliti kako povezati sve komponente u sustav koji bi imao neku namjenu. Stoga smo odlučili napraviti alarm sustav.

<span id="anchor-5"></span>2.2 Opis i zahtjevi sustava

Alarm sustav služi kao mjera sigurnosti protiv provala ili sumnjivih aktivnosti. Detektor zvuka se korisit za detekciju zvuka iz okoline, *active buzzer module* služi kao zvuk samog alarma, a pomoću *matrix* tipkala ostvaruje se sučelje između korisnika i sustava.

Program je podijeljen u tri načina rada: *Home mode*(*welcome screen*), *Set alarm password mode* i *Alarm on mode*. Sva interakcija između korisnika i sustava izvedena je pomoću tipkala i lcd ekrana. Pokretanjem programa otvara se *home mode* koji se i prikazuje na ekranu. Pritiskom na odgovarajuću tipku korisnik ima mogućnost izabrati opciju B(*Set alarm password*) ili opciju C(*Activate alarm*). Ulaskom u način rada B od korisnika se traži unos četveroznamenkaste lozinke koja će se koristiti za deaktivaciju alarma. Tijekom unosa lozinke nije moguće ući u druge načine rada. Nakon što je upisana lozinka program se vraća u početni način rada, A, odnosno *home screen*. Iz njega korisnik može opet promijeniti lozinku ili može aktivirati alarm, opcija C. Pokretanjem načina rada C sustav skenira zvuk iz okoline te ako zvuk prijeđe određeni prag jačine(amplitude) zvuka, alarm se pokreće. Korisnik ima 10 sekundi za upisati lozinku, kako bi se alarm zaustavio. Ukoliko Korisnik upiše krivu lozinku ili prođe 10 sekundi. Sustav simulira pozivanje policije i ulazi u konačni način rada gdje se alarm nikako ne može isključiti osim fizičkim gašenjem sustava.

***Home mode*****, A**

***Set password mode*****, B**

***Alarm on mode*****, C**

**Konačni način rada**

<span id="anchor-6"></span>**2.3 Opis komponenti**

<span id="anchor-7"></span>2.3.1 Atmel Atmega16 mikrokontroler

Atmega16 mikrokontroler je AVR arhitekture. Ima smanjeni broj instrukcija(RISC), 32 KB programirajuće flash memorije i 2 KB interne SRAM memorije. Sadrži 32 ulazno/izlazna pina koji su podjeljeni u 4 porta. Nudi i mogućnost korištenja timera i adc-a. Frekvencija rada je 7.3728 MHz, a radni je napon između 2.7V i 5.5V.

Mikrokontroler se može programirati u asembleru ili u C programskom jeziku. Zatim se programi prevode u .hex datoteku koja se preko programatora kopira u programsku memoriju na mikrokontroleru.

<span id="anchor-8"></span>2.3.2 LCD

LCD 1602A ekran je koji se koristi za prikaz teksta. Tekst se prikazuje u dva retka. Svaki redak može sadržavati 16 znakova. Podaci se primaju preko osambitnog ili četverobitnog ulaza. Vss i Vdd služe za napajanje, dok se kontrola s lcd-om izvršava preko V0, RS, R/W, E pinova. Zadnja četiri pina koriste se za podešavanje pozadinskog osvjetljenja. Softverska komunikacija s ekranom moguća je korištenjem lcd.h knjižnice funkcija.

Pinout LCD-a

<span id="anchor-9"></span>2.3.3 LM393 detektor zvuka

Detektor zvuka služi za detekciju jačine zvuka. Sastoji se od mikrofona koji prima analogni signal, pojačala, potenciometra i pinova. Pinovi su: GND(uzemljenje), +5V(napon) i DO(digitalni izlaz). Senzor nakon što zabilježi određenu jačinu zvuka, preko neke granice, pošalje logičku 0 na digitalni izlaz. Tako se ostvaruje komunikacija između detektora i mikrokontrolera. Potenciometar se koristi za podešavanje osjetljivsoti senzora, odnosno praga jačine zvuka nakon kojega će se poslati signal mikrokontroleru.

**LM393 Sensor**

<span id="anchor-10"></span>2.3.4 Matrix Tipkalo

Matrično tipkalo komponenta je koja uz LCD ekran omogućava sučelje između korisnika i sustava. Na tipkalu se nalazi 16 tipki. Brojevi od 0 do 9, \*, \#, i slova: A, B, C, D. Zbog uštede I/O pinova mikrokontrolera, svaka tipka iz tipkala nema svoj izlaz. Nego su izlazi podijeljeni u redove i stupce. Tako dobijemo 4 reda i 4 stupca. Svaki izlaz tipke na tipkalu odgovara jednoj od kombinacija reda i stupca. Na primjer: Tipka 2 odgovara kombinaciji prvog reda i drugog stupca. Nakon što je tipka stisnuta, na 8-bitnom izlazu pojavljuje se određena kombinacija 8 bitnog broja.

**Matrix tipkalo**

<span id="anchor-11"></span>2.3.5 *Active Buzzer*

*Active buzzer* komponenta je koja, kada na ulazu dobije logičku nulu, proizvodi glasan, konstantan zvuk iste frekvencije. Sastoji se od zvučnika i pinova: GND(uzemljenje), +5V(napon) i digitalni ulaz.

**Buzzer**

<span id="anchor-12"></span>

<span id="anchor-13"></span>3.OPIS RJEŠENJA

<span id="anchor-14"></span>3.1 Hardversko rješenje

Prvi korak u implementaciji projekta je bio hardversko rješenje. Odnosno međusobno spajanje svih komponenti. Zbog malog broja komponenti imali smo dovoljno pinova na svim portovima. Tako smo svakoj komponenti dodijelili jedan port mikrokontrolera. Tipkalo je spojeno na port b. Redovi tipkala spojeni su na zadnja 4 pina, a stupci na prva 4 pina. R0, R1, R2, R3 spojeni su na PB7, PB6, PB5, PB4, a stupci C0, C1, C2, C3 su spojeni na PB3, PB2, PB1, PB0.

Kontrolni port LCD ekrana je PORTD(pinovi 4-7), a *data* port je PORTA s prva 4 pina.

PORTC koristimo za *buzzer* i za detektor zvuka. Izlaz za *buzzer* na zadnjem je pinu porta, a ulaz detektora je na prvom pinu. Radi lakšeg spajanja LCD ekrana i *sound *detektora koristimo još i *breadboard*.

**Prikaz hardvera**

<span id="anchor-15"></span>3.2 Softversko rješenje

Softverska implementacija realizirana je kao više funkcija koje se pozivaju zavisno o trenutnom načinu rada. Na primjer, ako se korisnik prebaci u *Set password* način rada(B), tada se izvršava funckija koja incijalizira taj način rada te se nakon toga vrti u petlji kroz funkciju koja je zadužena za taj način rada.

Korištene knjižnice funkcija su: stdio.h, string.h koja sadrži sprintf() funkciju, util/delay.h, avr/io.h, i lcd.h koja sadrži implementaciju funkcija koje se koriste kao interface s LCD ekranom.

Program započinje main() funkcijom. Izvršava se inicijalizacija LCD ekrana. Inicijalizacija porta c, na kojem su spojeni *sound *detektor i *buzzer*. Prvi bit, na koji je spojen detektor, postavljamo kao ulazni, a sve ostale bitove kao izlazne. Zatim postavimo *pull up* otpornik na detektoru i postavimo logičku jedinicu na ulazu u *buzzer*. Slijedi prikazivanje *welcome *načina rada(A) na ekranu.

Glavni petlja programa sastoji se od dva dijela. *Polling *načina rada operacije i izvršavanja načina rada kojeg je korisnik izabrao. *Polling *je implementiran tako što se dohvaća pritisnuta tipka te se ovisno o pritisnutoj tipci pokreneće inicijalizacija moda i postavljanje varijable koja sadrži trenutni način rada.

Zatim se izvšrava funkcija izabranog načina rada u petlji.

Čitanje pritisnute tipke sa tipkala implementirano je u funkciji *getPressedKey()*. U funkciji se prvo čitaju redovi, a zatim stupci. Čitanje se ostvaruje postavljanjem DDRB registra kao ulaznog i podižu se pull up otpornici. Zatim se može pročitati vrijednost reda, odnosno stupca iz PINB registra. Kombinacija reda i stupca daje pritisnutu tipku te funkcija vraća *character*.

Ulaskom u funkciju *setAlarmPassword()* načina rada B, onemogućuje se *polling *modova te se čita string s tipkala, odnosno lozinka. Zatim je moguće vratiti se nazad na početni način rada ili upaliti alarm.

Funkcija načina rada C, cijelo vrijeme provjerava je li detektor zvuka poslao signal i u isto vrijeme čita znakove s *matrix *tipkala. Ako je prvi bit na pinu c u logičkoj nuli, ako je detektor poslao signal, alarm se pali. Zvuk alarma izveden je tako što se na ulazu u *buzzer *pošalje logička nula pa zatim nakon 100 ms se pošalje jedinica pa sljedeći put nakon isto 100 ms nula i tako dalje. Ako je *string *upisane lozinke dugačak 4 znaka, tada se string upisane lozinke uspoređuje s lozinkom koja je prethodno postavljena te ako su stringovi isti alarm se deaktivira.

Modaliteti blinkanja ledica u funkciji *robbery()* implementirani su manipulacijom port a registra. Prvi modalitet komplementira trenutno stanje registra. Gdje su, na početku, svi bitovi postavljeni na logičku jedninicu. Drugi modalitet, kao i prvi, komplementira port a registar, samo što je početno stanje registra drukčije, odnosno 0b01010101.

<span id="anchor-16"></span>4. OGRANIČENJA I POBOLJŠANJA

<span id="anchor-17"></span>4.1 detektor zvuka LM393

Detektor na sebi sadrži potenciometar koji omogućuje podešavanje praga jačine zvuka na koji će detektor poslati izlazni signal. Ukoliko je detektor preosjetljiv, slati će izlazni signal cijelo vrijeme, a ako je prag jako visko, neće se slati signal na nikakve zvučne signale.

Stoga smo morali naći sredinu osjetljivosti koja odgovara našoj primjeni detektora. Nažalost srednja je osjetljivost jako mala i trebalo nam je jako puno pokušaja i pogrešaka kako bismo je našli.

Drugo ograničenje detektora je smanjeni prostor primanja signala mikrofona. Zvuk mora dolaziti pod okomitim kutem na površinu mikrofona.

Zvuk mora biti konstantan u amplitudi i trajanju kako bi se registrirao u detektoru.

<span id="anchor-18"></span>5. ZAKLJUČAK

Radeći na ovom projektu iskoristili smo i poboljšali vještine analize problema, pronalaska rješenja i krajnje implementacije sustava. Internet nam je poslužio kao dobar alat u pronalasku informacija o komponentama i programiranju mikrokontrolera.

Poveznica na repozitorij: [*https://github.com/drokandino/alarmSystem*](https://github.com/drokandino/alarmSystem)

<span id="anchor-19"></span>5. REFERENCE

\[1\] - Sučelje između atmege i *matrix *tipkala - [*https://circuitdigest.com/microcontroller-projects/keypad-interfacing-with-avr-atmega32*](https://circuitdigest.com/microcontroller-projects/keypad-interfacing-with-avr-atmega32)

\[2\] - Sučelje između atmege i *matrix *tipkala - [*https://embedds.com/interfacing-matrix-keyboard-with-avr/*](https://embedds.com/interfacing-matrix-keyboard-with-avr/)

\[3\] - Korištenje LM393 detektora - [*https://randomnerdtutorials.com/guide-for-microphone-sound-sensor-with-arduino/*](https://randomnerdtutorials.com/guide-for-microphone-sound-sensor-with-arduino/)

\[4\] - Iskustva drugih kupaca LM393 detektora -

[*https://www.amazon.com/Detection-Sensor-Intelligent-Vehicle-Arduino/dp/B00E8D67H0/ref=cm\_cr\_arp\_d\_product\_top?ie=UTF8*](https://www.amazon.com/Detection-Sensor-Intelligent-Vehicle-Arduino/dp/B00E8D67H0/ref=cm_cr_arp_d_product_top?ie=U)

\[5\] - Korištenje *active buzzer* modula - [*https://fut688.en.made-in-china.com/product/uNYQrEeyZOVo/China-Active-Buzzer-Module-High-Level-Trigger-5V-for-Arduino-AVR-Pic.html*](https://fut688.en.made-in-china.com/product/uNYQrEeyZOVo/China-Active-Buzzer-Module-High-Level-Trigger-5V-for-Arduino-AVR-Pic.html)

\[6\] - Atmega-16 *datasheet *- [*http://ww1.microchip.com/downloads/en/devicedoc/doc2466.pdf*](http://ww1.microchip.com/downloads/en/devicedoc/doc2466.pdf)
