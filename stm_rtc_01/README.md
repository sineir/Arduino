# stm32f103 rtc time

Relogio digital stm32f103+max7219
- IDE Arduino 1.8.5
- [Core](https://github.com/stm32duino/Arduino_Core_STM32)
- [Biblioteca STM32RTC](https://github.com/stm32duino/STM32RTC)
- Necessita bateria no pin 'VBAT'

Objetivos:
1. Elaboraçao sketch básico. **ok 11/08/2018**
2. Adicionar botões para ajustes
3. Adicionar IR para ajuste via controle
4. Adicionar funçoes alarme

## Problemas
- Adiantando 01 minuto a cada 15 minutos (parece que a linha abaixo resolveu).
  - rtc.setClockSource(STM32RTC::RTC_HSE_CLOCK);
- Ao desligar e ligar o relógio "zera".
  - Mesmo ao energizar vbat com 3.3 permanente não resolveu. Pendente..
