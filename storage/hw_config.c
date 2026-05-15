#include "hw_config.h"

#include "ff.h"
#include "f_util.h"

#include "hw_config.h"
#include "sd_card.h"
#include "spi.h"

#include "hardware/spi.h"

static spi_t spis[] = {
    {
        .hw_inst = spi1,

        .miso_gpio = 12,
        .mosi_gpio = 11,
        .sck_gpio = 10,

        .baud_rate = 1000 * 1000,

        .set_drive_strength = false
    }
};

static sd_card_t sd_cards[] = {
    {
        .pcName = "0:",

        .spi = &spis[0],

        .ss_gpio = 13,

        .use_card_detect = false,
        .card_detect_gpio = 0,
        .card_detected_true = 0
    }
};

size_t spi_get_num() {
    return count_of(spis);
}

spi_t *spi_get_by_num(size_t num) {
    return &spis[num];
}

size_t sd_get_num() {
    return count_of(sd_cards);
}

sd_card_t *sd_get_by_num(size_t num) {
    return &sd_cards[num];
}