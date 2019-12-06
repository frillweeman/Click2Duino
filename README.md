# Click2Duino
Counterfeit Click2Enter Device for a fraction of the price

## Parts
- Arduino (Uno, Nano, etc.)
- RGB LED
- 433 MHz Receiver Module
- Relay
- Jumper Wires & Breadboard
- Security Wire (for connecting to gate control module)
- Magnet (for mounting)

## Improvements
This code was written to meet C2E specs with some improvements.
- If user holds down PTT for too long, the LED turns orange for as long as the button is held down or 2 seconds minimum. This helps with troubleshooting in cases where there is interference. If the LED were left off, it could be mistaken for a power issue.
- Use of a single RGB LED instead of two separate LEDs.
