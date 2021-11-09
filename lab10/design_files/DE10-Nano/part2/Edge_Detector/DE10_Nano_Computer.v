
module DE10_Nano_Computer (
	////////////////////////////////////
	// FPGA Pins
	////////////////////////////////////

	// Clock pins
	input              CLOCK_50,
	input              CLOCK2_50,
	input              CLOCK3_50,
	
	// HDMI
	inout              HDMI_I2C_SCL,
	inout              HDMI_I2C_SDA,
	inout              HDMI_I2S,
	inout              HDMI_LRCLK,
	inout              HDMI_MCLK,
	inout              HDMI_SCLK,
	output             HDMI_TX_CLK,
	output      [23:0] HDMI_TX_D,
	output             HDMI_TX_DE,
	output             HDMI_TX_HS,
	input              HDMI_TX_INT,
	output             HDMI_TX_VS,

	////////////////////////////////////
	// HPS Pins
	////////////////////////////////////
	
	// DDR3 SDRAM
	output      [14:0] HPS_DDR3_ADDR,
	output      [2:0]  HPS_DDR3_BA,
	output             HPS_DDR3_CAS_N,
	output             HPS_DDR3_CKE,
	output             HPS_DDR3_CK_N,
	output             HPS_DDR3_CK_P,
	output             HPS_DDR3_CS_N,
	output      [3:0]  HPS_DDR3_DM,
	inout       [31:0] HPS_DDR3_DQ,
	inout       [3:0]  HPS_DDR3_DQS_N,
	inout       [3:0]  HPS_DDR3_DQS_P,
	output             HPS_DDR3_ODT,
	output             HPS_DDR3_RAS_N,
	output             HPS_DDR3_RESET_N,
	input              HPS_DDR3_RZQ,
	output             HPS_DDR3_WE_N,

	// Ethernet
	output             HPS_ENET_GTX_CLK,
	inout              HPS_ENET_INT_N,
	output             HPS_ENET_MDC,
	inout              HPS_ENET_MDIO,
	input              HPS_ENET_RX_CLK,
	input       [3:0]  HPS_ENET_RX_DATA,
	input              HPS_ENET_RX_DV,
	output      [3:0]  HPS_ENET_TX_DATA,
	output             HPS_ENET_TX_EN,

	// Accelerometer
	inout              HPS_GSENSOR_INT,

	// I2C
	inout              HPS_I2C0_SCLK,
	inout              HPS_I2C0_SDAT,
	inout              HPS_I2C1_SCLK,
	inout              HPS_I2C1_SDAT,
	
	// Pushbutton
	inout              HPS_KEY,

	// LED
	inout              HPS_LED,

	// LTC
	inout              HPS_LTC_GPIO,

	// SD Card
	output             HPS_SD_CLK,
	inout              HPS_SD_CMD,
	inout       [3:0]  HPS_SD_DATA,
	
	// SPI
	output             HPS_SPIM_CLK,
	input              HPS_SPIM_MISO,
	output             HPS_SPIM_MOSI,
	inout              HPS_SPIM_SS,
	
	// UART
	input              HPS_UART_RX,
	output             HPS_UART_TX,
	
	// USB
	inout              HPS_CONV_USB_N,
	input              HPS_USB_CLKOUT,
	inout       [7:0]  HPS_USB_DATA,
	input              HPS_USB_DIR,
	input              HPS_USB_NXT,
	output             HPS_USB_STP
);


//=======================================================
//  REG/WIRE declarations
//=======================================================
wire  hps_fpga_reset_n;




//=======================================================
//  Structural coding
//=======================================================

Computer_System The_System (
	////////////////////////////////////
	// FPGA Side
	////////////////////////////////////

	// Global signals
	.system_pll_ref_clk_clk				(CLOCK_50),
	.system_pll_ref_reset_reset			(1'b0),
	.vga_ref_clock_clk					(CLOCK2_50),
	.vga_ref_reset_reset				(1'b0),

	////////////////////////////////////
	// HPS Side
	////////////////////////////////////
	// DDR3 SDRAM
	.memory_mem_a			            (HPS_DDR3_ADDR),
	.memory_mem_ba			            (HPS_DDR3_BA),
	.memory_mem_ck			            (HPS_DDR3_CK_P),
	.memory_mem_ck_n		            (HPS_DDR3_CK_N),
	.memory_mem_cke			            (HPS_DDR3_CKE),
	.memory_mem_cs_n		            (HPS_DDR3_CS_N),
	.memory_mem_ras_n		            (HPS_DDR3_RAS_N),
	.memory_mem_cas_n		            (HPS_DDR3_CAS_N),
	.memory_mem_we_n		            (HPS_DDR3_WE_N),
	.memory_mem_reset_n		            (HPS_DDR3_RESET_N),
	.memory_mem_dq			            (HPS_DDR3_DQ),
	.memory_mem_dqs			            (HPS_DDR3_DQS_P),
	.memory_mem_dqs_n		            (HPS_DDR3_DQS_N),
	.memory_mem_odt			            (HPS_DDR3_ODT),
	.memory_mem_dm			            (HPS_DDR3_DM),
	.memory_oct_rzqin		            (HPS_DDR3_RZQ),

    // Accelerometer
	.hps_io_hps_io_gpio_inst_GPIO61		(HPS_GSENSOR_INT),

	// Ethernet
	.hps_io_hps_io_gpio_inst_GPIO35		(HPS_ENET_INT_N),
	.hps_io_hps_io_emac1_inst_TX_CLK	(HPS_ENET_GTX_CLK),
	.hps_io_hps_io_emac1_inst_TXD0		(HPS_ENET_TX_DATA[0]),
	.hps_io_hps_io_emac1_inst_TXD1		(HPS_ENET_TX_DATA[1]),
	.hps_io_hps_io_emac1_inst_TXD2		(HPS_ENET_TX_DATA[2]),
	.hps_io_hps_io_emac1_inst_TXD3		(HPS_ENET_TX_DATA[3]),
	.hps_io_hps_io_emac1_inst_RXD0		(HPS_ENET_RX_DATA[0]),
	.hps_io_hps_io_emac1_inst_MDIO		(HPS_ENET_MDIO),
	.hps_io_hps_io_emac1_inst_MDC		(HPS_ENET_MDC),
	.hps_io_hps_io_emac1_inst_RX_CTL	(HPS_ENET_RX_DV),
	.hps_io_hps_io_emac1_inst_TX_CTL	(HPS_ENET_TX_EN),
	.hps_io_hps_io_emac1_inst_RX_CLK	(HPS_ENET_RX_CLK),
	.hps_io_hps_io_emac1_inst_RXD1		(HPS_ENET_RX_DATA[1]),
	.hps_io_hps_io_emac1_inst_RXD2		(HPS_ENET_RX_DATA[2]),
	.hps_io_hps_io_emac1_inst_RXD3		(HPS_ENET_RX_DATA[3]),

	// I2C
	.hps_io_hps_io_i2c0_inst_SDA		(HPS_I2C0_SDAT),
	.hps_io_hps_io_i2c0_inst_SCL		(HPS_I2C0_SCLK),
	.hps_io_hps_io_i2c1_inst_SDA		(HPS_I2C1_SDAT),
	.hps_io_hps_io_i2c1_inst_SCL		(HPS_I2C1_SCLK),

	// Pushbutton
	.hps_io_hps_io_gpio_inst_GPIO54		(HPS_KEY),

	// LED
	.hps_io_hps_io_gpio_inst_GPIO53		(HPS_LED),

	// VGA Subsystem
	.vga_CLK							(HDMI_TX_CLK),
	.vga_BLANK							(HDMI_TX_DE),
	.vga_SYNC							(),
	.vga_HS								(HDMI_TX_HS),
	.vga_VS								(HDMI_TX_VS),
	.vga_R								(HDMI_TX_D[23:16]),
	.vga_G								(HDMI_TX_D[15:8]),
	.vga_B								(HDMI_TX_D[7:0]),

	// LTC
	.hps_io_hps_io_gpio_inst_GPIO40		(HPS_LTC_GPIO),

	// SD Card
	.hps_io_hps_io_sdio_inst_CMD		(HPS_SD_CMD),
	.hps_io_hps_io_sdio_inst_D0			(HPS_SD_DATA[0]),
	.hps_io_hps_io_sdio_inst_D1			(HPS_SD_DATA[1]),
	.hps_io_hps_io_sdio_inst_CLK		(HPS_SD_CLK),
	.hps_io_hps_io_sdio_inst_D2			(HPS_SD_DATA[2]),
	.hps_io_hps_io_sdio_inst_D3			(HPS_SD_DATA[3]),

	// SPI
	.hps_io_hps_io_spim1_inst_CLK		(HPS_SPIM_CLK),
	.hps_io_hps_io_spim1_inst_MOSI		(HPS_SPIM_MOSI),
	.hps_io_hps_io_spim1_inst_MISO		(HPS_SPIM_MISO),
	.hps_io_hps_io_spim1_inst_SS0		(HPS_SPIM_SS),

	// UART
	.hps_io_hps_io_uart0_inst_RX		(HPS_UART_RX),
	.hps_io_hps_io_uart0_inst_TX		(HPS_UART_TX),

	// USB
	.hps_io_hps_io_gpio_inst_GPIO09		(HPS_CONV_USB_N),
	.hps_io_hps_io_usb1_inst_D0			(HPS_USB_DATA[0]),
	.hps_io_hps_io_usb1_inst_D1			(HPS_USB_DATA[1]),
	.hps_io_hps_io_usb1_inst_D2			(HPS_USB_DATA[2]),
	.hps_io_hps_io_usb1_inst_D3			(HPS_USB_DATA[3]),
	.hps_io_hps_io_usb1_inst_D4			(HPS_USB_DATA[4]),
	.hps_io_hps_io_usb1_inst_D5			(HPS_USB_DATA[5]),
	.hps_io_hps_io_usb1_inst_D6			(HPS_USB_DATA[6]),
	.hps_io_hps_io_usb1_inst_D7			(HPS_USB_DATA[7]),
	.hps_io_hps_io_usb1_inst_CLK		(HPS_USB_CLKOUT),
	.hps_io_hps_io_usb1_inst_STP		(HPS_USB_STP),
	.hps_io_hps_io_usb1_inst_DIR		(HPS_USB_DIR),
	.hps_io_hps_io_usb1_inst_NXT		(HPS_USB_NXT)
);

I2C_HDMI_Config u_I2C_HDMI_Config (
	.iCLK								(CLOCK3_50),
	.iRST_N								(1'b1),
	.I2C_SCLK							(HDMI_I2C_SCL),
	.I2C_SDAT							(HDMI_I2C_SDA),
	.HDMI_TX_INT						(HDMI_TX_INT)
);

endmodule

