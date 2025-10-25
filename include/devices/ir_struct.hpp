#pragma once

enum IR_DECODE_TYPE {
    TYPE_NEC
};

struct s_IRData {
    IR_DECODE_TYPE protocol;
    uint8_t addr;
    uint8_t command;
};