# Example 3: FieldBus19 Controller on a STM32F401 Nucleo Board

**Connections:**
* Connect J1 of the FB19 Controller node with J2 of one of the FB19 Subscriber nodes
* Connect J2 of the FB19 Controller node with J2 of the other FB19 Subscriber node

**Functionality:**
* If you push the push button PB1 on the FB19 Controller's NEB, LED1 on both Subscriber's NEBs will light up
* If you push the push button PB1 on the FB19 Subscriber1's NEB, LED1 on the FB19 Subscriber2's NEB and LED1 on the FB19 Controller's NEB will light up
* If you push the push button PB1 on the FB19 Subscriber2's NEB, LED1 on the FB19 Subscriber1's NEB and LED1 on the FB19 Controller's NEB will light up
* Note: In order to reduce console output, just 2 messages per second are sent to the FB19 Subscribers

If you encounter any problems, please contact us at info@bitcontrol.ch.
