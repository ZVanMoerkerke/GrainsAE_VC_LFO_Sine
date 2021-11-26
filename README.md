# GrainsAE_VC_LFO_Sine
Low frequency sine wave oscillator with voltage control for Grains by Keurslager Kurt / Zeno Van Moerkerke (26/11/2021)

Quite straightforward script to run a sine wave LFO on AE Grains / Arduino Nano. Commentary is placed in the sketch for those interested in the inner workings of it. On more info on how to upload it to your Grains, please refer to the AE Modular Wiki page on the module.

Controls are as follows:

POT1 / CV1: control over frequency, ranging from 0.1Hz up to about 10Hz.
POT2 / CV2: control the amplitude of the sine wave. Note that due to the digital nature of Grains, at small amplitude, there are only a few 'steps' in the sinewave. For higher quality, it is better to attenuate with an external, analog module.
POT3: controls the offset of the sine wave. You can change the center of the sine wave with this knob. If the sinewave hits 0 or 5V this way, it will get topped off.
CV3: this is the SYNC functionality. A trigger or high pulse here will reset the sinewave to its center position & starting going up.

Possible future improvements:
Higher max speed (needs more efficient coding for that).
Do something with the audio input of Grains (input 4)?
..

Let me know if you have any comments / improvement ideas!
And if you make music with this, I'd love to hear it, feel free to tag me anywhere :) 

Cheers

Zeno / Keurslager Kurt
