/**
 * \file
 *
 * \brief User board configuration template
 *
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

/** SPI MACRO definition */
#define CONF_BOARD_SPI

/** SPI slave select MACRO definition */
#define CONF_BOARD_SPI_NPCS0

/** Spi Hw ID . */
#define SPI_ID          ID_SPI

/** SPI base address for SPI master mode*/
#define SPI_MASTER_BASE      SPI
/** SPI base address for SPI slave mode, (on different board) */
#define SPI_SLAVE_BASE       SPI

/* Chip select. */
#define SPI_CHIP_SEL 0

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 1

/* Delay before SPCK. */
#define SPI_DLYBS 0x40
//#define SPI_DLYBS 0x0

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x10


__attribute__( ( always_inline ) ) __STATIC_INLINE void nop(void)
{
	__ASM volatile ("nop");
}

#define PAL_WAIT_1_US() {nop(); nop(); nop(); nop(); \
nop(); nop(); nop(); nop(); \
nop(); nop(); nop(); nop(); \
nop(); nop(); nop(); nop(); \
nop(); nop(); nop(); nop(); \
nop(); nop(); nop(); nop(); \
nop(); nop(); nop(); nop(); \
nop(); nop(); nop(); nop();}

//#define PHY_AT86RF231
/** IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0

#endif // CONF_BOARD_H
