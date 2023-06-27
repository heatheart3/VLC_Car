# Ver1.0
low-speed VLC scheme

# Ver1.1
Do some refractor stuff of the Ver1.0, more readable.

# Ver2.1
1. Intergrated all encoding scheme and use OOK demodulation.
    - **Spinalcode->Network coding->Manchester Encode->OOK**

2. Some params:
    - Transmitter frequency: 5KHz
    - ADC sampling rate: 100KHz
    - SpinalCode: k=3,B=2,c=6
    - network coding:XOR

## Interesting stuff
1. Decoding is much faster than ADC sampling.
2. Using ADC to demodulate OOK is not precise, therefore the next step is to use GPIO to demodulate.