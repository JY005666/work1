#include <stdint.h>
void stepper_init(void);

void stepper_step(uint32_t steps, uint8_t direction, uint16_t speed);
