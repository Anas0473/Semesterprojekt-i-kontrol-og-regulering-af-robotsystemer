# Semesterprojekt i kontrol og regulering af robotsystemer

Manuelt styret pan-tilt system udviklet som et overvågningskamera med encoder-feedback. Projektet anvender SPI-kommunikation mellem en Tiva C mikrokontroller og et Pynq Z2 FPGA-board. Systemet kombinerer RTOS, VHDL og H-bro-motorstyring med åben sløjfe-regulering.

---

## Projektoversigt

### Tiva-kontrol og SPI-kommunikation  
Tiva C fungerer som master og genererer PWM-signaler til motorstyring, samtidig med at den kommunikerer med FPGA’en via SPI. Koden er skrevet i C med FreeRTOS for opgavehåndtering og realtidssynkronisering.

### Pynq FPGA som slave  
FPGA’en håndterer motorernes retningsstyring og encoder-feedback. Systemet modtager SPI-kommandoer, aflæser encoder-signaler og sender vinkler tilbage til Tiva’en via SPI. Alt er implementeret i VHDL i Vivado.

### Åben kontrolsløjfe  
Vinkler for pan og tilt vises uden feedbackstyring. Der er udarbejdet teoretisk grundlag for PID-regulering, men det blev ikke implementeret grundet tid. Systemet blev testet med encoder-aflæsning og PWM-output fra Tiva.

### Hardware og elektronik  
- Motorstyring via dobbelte H-broer  
- EMG30-motorer med inkrementelle encodere  
- SPI-interface med matchende clock og dataformat  
- Encoderbeskyttelse og spændingsregulering dokumenteret

---

## Inkluderede filer

- SEMESTER_4_PROJECT.pdf – Fuld projektbeskrivelse med metode og eksperimenter  
- C code for the Master Tiva/ – C-kode til SPI-kommunikation og PWM med FreeRTOS  
- VHDL code for Slave PYNQ Z2/ – VHDL-kode til FPGA-moduler og encoderhåndtering  
- Dual H-bro_V2.PDF – Dokumentation for motorstyringskredsløb  
- PYNQ_Z2_User_Manual_v1.pdf – Manual for FPGA-board  
- TivaTM4C123GH6PMMicrocontrollerDatasheet.pdf – Datablad for mikrokontrolleren  
- EMG30_protection.PDF – Kredsløbsbeskyttelse for motor og encoder

---

## Udførte eksperimenter

- Aflæsning og validering af pan- og tilt-vinkler fra encoder  
- Måling af afvigelse mellem forventet og faktisk position (0,36 % fejl)  
- Test af PWM-spændingsudgang fra Tiva (8,15 % afvigelse)  
- Verifikation af SPI-kommunikation mellem master og slave

---

## Kompetencer

- Programmering af mikrokontroller med FreeRTOS og SPI  
- Udvikling af FPGA-moduler i VHDL til realtime databehandling  
- Motorstyring via PWM og dobbelte H-broer  
- Inkrementel encoderhåndtering og vinkelberegning  
- Systemintegration mellem mikrokontroller og FPGA  
- Teknisk dokumentation og fejlsøgning i hardware/software

---

## Bidragsydere

- Anas Butt Hussain  
- Alan Rashid  
- Jakub Hubert Rudowski  
- Patrick Ørsted Povey Andersen  
- Robin Hansen  
- Thomas Vilholm  
- Peter Jøker Trachsel

---

## Intention

Dette projekt blev afleveret som en del af faget "Kontrol og regulering af robotsystemer" på Syddansk Universitet og deles her med det formål at demonstrere færdigheder inden for mikrokontrollerprogrammering, VHDL-udvikling og SPI-kommunikation som en del af en professionel portefølje.
