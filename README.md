#DSPController


__DSPController__ is an embedded human interface device that connects to the _Analog Devices_ __SHARC ADSP-21364__ evaluation board via SPI to upgrade its flexibility. The __DSPController__ bundle contains the hardware itself and the software API which can be used in the VisualDSP++ environment with the supported DSP chips.



_This repository contains a project that uses the lates DSPController driver and  demonstartes the basic usage of the  DSPController API._

---

### How to use this project

There are three files you should care about:

- __INTERFACE.c__ - _implements the interface state machine_
- __GLUE.h__ - _connects INTERFACE.c to DSP.c_
- __DSP.c__ - _implements the signal processing algorithm_

The project contains a bit more advanced application that implements

- an interface demo, that shows how to use the DSPController API
- an audio application that contains a signal generator, a resonant low pass filter and an output multiplexer that can route the generated/filetered signal to arbitrary outputs of the DSP evaluation card.

The __INTERFACE.c__ file demonstrates the easiest method to implement a menu based complex human interface with a state machine. __GLUE.h__ demonstrates how to work with global variables and it's initial values.

---

### Check out the whole boundle

[DSPController Bundle](https://github.com/tiborsimon/DSPController-Bundle)





