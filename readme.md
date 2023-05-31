Turns your M5StickC into a CUE-Light for theater or show cueing. To be controlled with any OSC capable Software such as LinuxShowPlayer, Qlab, Chataigne or many others.

IP Adress and OSC-Path are shown on Startup. Startup completed when LED turns of and Display is red.

/oscuelight/[ID]/color [red;green,blue]
/oscuelight/[ID]/write [INT] or [STRING]// writes the first two Digits or first 8 characters to the Display - has to be deleted with e.g. color
/oscuelight/[ID]/achtung [CUENUMBER] or [STRING] // flashes the Display 3 Times and turns LED ON  - shows first two digits of an integer or draws up to 70 characters of a string - Display turns green
/oscuelight/[ID]/go //flashes the Display 3 Times and turns off LED - Display turns red