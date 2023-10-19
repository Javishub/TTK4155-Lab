#include "pwm.h"

#include "sam.h"

// pulse width 0.9ms - 2.1ms
// signal period = 20ms
// duty cycle = 0.9/20 <-> 2.1 / 20
// duty cycle = 0.045 <-> 0.105

static const uint32_t CLK_PRESCALAR = 1024;
static const double PERIOD = 20.0 / 1000; // 20ms
static double F_CHANNEL_5_CLOCK = CHIP_FREQ_CPU_MAX / CLK_PRESCALAR;
static uint32_t CPRD = PERIOD * CHIP_FREQ_CPU_MAX / CLK_PRESCALAR;
static const double MIN_DUTY_CYCLE = 0.9 / 20;
static const double MAX_DUTY_CYCLE = 2.1 / 20;

void pwm_init()
{
    /*
        * Power Management p. 974

        The PWM is not continuously clocked. The programmer must first enable the PWM clock in the Power
        Management Controller (PMC) before using the PWM. However, if the application does not require PWM
        operations, the PWM clock can be stopped when not needed and be restarted later. In this case, the PWM will
        resume its operations where it left off.
        In the PWM description, Master Clock (MCK) is the clock of the peripheral bus to which the PWM is connected.
    */
    // enable power clock for pwm
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
    {
    };

    // Multiplex PIOC19 to peripheral
    PIOC->PIO_PDR |= PIO_PDR_P19;
    // set peripheral B as per page 973
    PIOC->PIO_ABSR |= PIO_ABSR_P19;
    /*
        * PWM Clock Generator p. 976

        The PWM master clock (MCK) is divided in the clock generator module to provide different clocks available for all
        channels. Each channel can independently select one of the divided clocks.

        Each linear divider can independently divide one of the clocks of the modulo n counter. The selection of the clock
        to be divided is made according to the PREA (PREB) field of the PWM Clock register (PWM_CLK). The resulting
        clock clkA (clkB) is the clock selected divided by DIVA (DIVB) field value.

        After a reset of the PWM controller, DIVA (DIVB) and PREA (PREB) are set to 0. This implies that after reset clkA
        (clkB) are turned off.

        Each channel selects and
        uses one of the clocks provided by the clock generator. The clock generator provides several clocks resulting from
        the division of the PWM master clock (MCK).
    */
    // clock A prescaler X = 1024, use prescalar A
    PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0b1010));

    /*
        the waveform polarity. At the beginning of the period, the signal can be at high or low level. This property is
        defined in the CPOL field of the PWM_CMRx register. By default the signal starts by a low level.
    */
    // channel A prescaler, left alignment, inverse polarity
    PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
    /*
        the waveform period. This channel parameter is defined in the CPRD field of the PWM_CPRDx register.
        If the waveform is left aligned, then the output waveform period depends on the counter source clock and
        can be calculated:
        By using the PWM master clock (MCK) divided by an X given prescaler value (with X being 1, 2, 4, 8, 16, 32,
        64, 128, 256, 512, or 1024), the resulting period formula will be:

        period = (X * CPRD) / MCK
        <=>
        CPRD = (period * MCK )/X
    */
    PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
    PWM->PWM_ENA = PWM_ENA_CHID5;

    return;
}

void pwm_set_duty_cycle(double duty_cycle)
{
    if (duty_cycle < MIN_DUTY_CYCLE)
    {
        duty_cycle = MIN_DUTY_CYCLE;
    }
    if (duty_cycle > MAX_DUTY_CYCLE)
    {
        duty_cycle = MAX_DUTY_CYCLE;
    }

    /*
        the waveform duty-cycle. This channel parameter is defined in the CDTY field of the PWM_CDTYx
        register.
        If the waveform is left aligned then:

        duty_cycle = (period - 1/(f_channel_1_clock * CDTY))/ period
        <=>
        CDTY = -1/(f_channel_1_clock * (duty_cycle * period - period))
     */
    PWM->PWM_CH_NUM[5].PWM_CMR |= PWM_CMR_CPOL;
    PWM->PWM_CH_NUM[5].PWM_CDTY = -1 / (F_CHANNEL_5_CLOCK * (duty_cycle * PERIOD - PERIOD));
}
