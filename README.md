# Click2Duino

Click2Duino is an open source, low-cost emergency vehicle gate entry device. It is designed to be an inexpensive alternative to devices such as [Click2Enter](http://www.click2enter.net/) and siren activated gate operating devices.

## Parts
- Arduino (Uno, Nano, etc.)
- RGB LED
- 433 MHz Receiver Module
- Relay
- Jumper Wires & Breadboard
- Security Wire (for connecting to gate control module)
- Mounting hardware

## Improvements
This code was written to meet [C2E](http://www.click2enter.net/) specs with some improvements.
- If user holds down PTT for too long, the LED turns orange for as long as the button is held down or 2 seconds minimum. This helps with troubleshooting in cases where there is interference. If the LED were left off, it could be mistaken for a power issue.
- Use of a single RGB LED instead of two separate LEDs.
