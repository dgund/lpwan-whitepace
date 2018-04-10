# lpwan-whitepace
Research for low-power wide-area networks (LPWAN) in open white space frequencies. Follows my research into the [policy implications of scaling LPWAN over white spaces](https://dgund.com/projects/lpwan-policy).

This experiment transmits/receives packets using a [LoRa](https://en.wikipedia.org/wiki/LoRa) radio at different frequencies and measures the received signal's relative strength index (RSI) and signal-to-noise ratio (SNR). This experiment is intended to be carried out with different environments, distances, and antennas in order to analyze the effectiveness of an LPWAN network at different white space frequencies.

Built for the [LoRaBug](https://github.com/OpenChirp/LoRaBug) CC2650+SX1276 radio from the [OpenChirp](https://openchirp.io) project at Carnegie Mellon University. Requires [TI-RTOS](http://www.ti.com/tool/TI-RTOS-MCU) and the [LoRaBug Firmware](https://github.com/OpenChirp/LoRaBug_Firmware) (which includes [LoraMac-node](https://github.com/Lora-net/LoRaMac-node)). Platform support provided thanks to the OpenChirp project's [Anthony Rowe](http://users.ece.cmu.edu/~agr/) and [Craig Hesling](https://github.com/linux4life798).
