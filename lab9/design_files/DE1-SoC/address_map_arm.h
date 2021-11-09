/* Memory */
#define DDR_BASE              0x00000000
#define DDR_SPAN              0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_SPAN        0x0000FFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_SPAN            0x03FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_SPAN      0x0003FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_SPAN        0x00001FFF

/* Cyclone V FPGA devices */
#define LW_BRIDGE_BASE			0xFF200000

#define LEDR_BASE             0x00000000
#define HEX3_HEX0_BASE        0x00000020
#define HEX5_HEX4_BASE        0x00000030
#define SW_BASE               0x00000040
#define KEY_BASE              0x00000050
#define GPIO0_BASE            0x00000060
#define GPIO1_BASE            0x00000070
#define PS2_BASE              0x00000100
#define PS2_DUAL_BASE         0x00000108
#define JTAG_UART_BASE        0x00001000
#define JTAG_UART_2_BASE      0x00001008
#define IrDA_BASE             0x00001020
#define TIMER0_BASE           0x00002000
#define TIMER1_BASE           0x00002020
#define AV_CONFIG_BASE        0x00003000
#define PIXEL_BUF_CTRL_BASE   0x00003020
#define CHAR_BUF_CTRL_BASE    0x00003030
#define AUDIO_BASE            0x00003040
#define VIDEO_IN_BASE         0x00003060
#define ADC_BASE              0x00004000

#define LW_BRIDGE_SPAN			0x00005000

/* ARM Peripherals */
#define I2C0_BASE					0xFFC04000		// base
#define I2C0_CON              0x00000000		// word offset
#define I2C0_TAR              0x00000001		// word offset
#define I2C0_DATA_CMD         0x00000004		// word offset
#define I2C0_FS_SCL_HCNT      0x00000007		// word offset
#define I2C0_FS_SCL_LCNT      0x00000008		// word offset
#define I2C0_ENABLE           0x0000001B		// word offset
#define I2C0_RXFLR            0x0000001E		// word offset
#define I2C0_ENABLE_STATUS    0x00000027		// word offset
#define I2C0_SPAN					0x00000100		// span

#define SYSMGR_BASE				0xFFD08000		// base
#define SYSMGR_GENERALIO7     0x00000127		// word offset
/* GENERALIO7 (trace_d6): 
    0 : Pin is connected to GPIO/LoanIO number 55. 
    1 : Pin is connected to Peripheral signal I2C0.SDA. 
    2 : Pin is connected to Peripheral signal SPIS1.SS0. 
    3 : Pin is connected to Peripheral signal TRACE.D6. */
#define SYSMGR_GENERALIO8     0x00000128		// word offset
/* GENERALIO8 (trace_d7): 
    0 : Pin is connected to GPIO/LoanIO number 56. 
    1 : Pin is connected to Peripheral signal I2C0.SCL. 
    2 : Pin is connected to Peripheral signal SPIS1.MISO. 
    3 : Pin is connected to Peripheral signal TRACE.D7. */
#define SYSMGR_I2C0USEFPGA    0x000001C1
/* I2C0USEFPGA: 
    0 : I2C0 uses HPS Pins. 
    1 : I2C0 uses the FPGA Inteface. */
#define SYSMGR_SPAN				0x00000800		// base
