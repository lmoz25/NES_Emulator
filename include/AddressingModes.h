#pragma once

enum class AddressingMode {
    ZERO_PAGE,
    PRE_INDEXED_INDIRECT,
    IMMEDIATE,
    ABSOLUTE,
    POST_INDEXED_INDIRECT,
    ZERO_PAGE_INDEXED,
    INDEXED,
    IMPLIED
};