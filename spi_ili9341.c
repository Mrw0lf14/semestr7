#include "spi_ili9341.h"
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void TFT9341_ini(uint16_t w_size, uint16_t h_size)
{
  uint8_t data[15];
  CS_ACTIVE();
  TFT9341_reset();
  //Software Reset
  TFT9341_SendCommand(0x01);
  HAL_Delay(1000);

  //Power Control A
  data[0] = 0x39;
  data[1] = 0x2C;
  data[2] = 0x00;
  data[3] = 0x34;
  data[4] = 0x02;
  TFT9341_SendCommand(0xCB);
  TFT9341_WriteData(data, 5);
  //Power Control B
  data[0] = 0x00;
  data[1] = 0xC1;
  data[2] = 0x30;
  TFT9341_SendCommand(0xCF);
  TFT9341_WriteData(data, 3);
  //Driver timing control A
  data[0] = 0x85;
  data[1] = 0x00;
  data[2] = 0x78;
  TFT9341_SendCommand(0xE8);
  TFT9341_WriteData(data, 3);
  //Driver timing control B
  data[0] = 0x00;
  data[1] = 0x00;
  TFT9341_SendCommand(0xEA);
  TFT9341_WriteData(data, 2);
  //Power on Sequence control
  data[0] = 0x64;
  data[1] = 0x03;
  data[2] = 0x12;
  data[3] = 0x81;
  TFT9341_SendCommand(0xED);
  TFT9341_WriteData(data, 4);
  //Pump ratio control
  data[0] = 0x20;
  TFT9341_SendCommand(0xF7);
  TFT9341_WriteData(data, 1);
  //Power Control,VRH[5:0]
  data[0] = 0x10;
  TFT9341_SendCommand(0xC0);
  TFT9341_WriteData(data, 1);
  //Power Control,SAP[2:0];BT[3:0]
  data[0] = 0x10;
  TFT9341_SendCommand(0xC1);
  TFT9341_WriteData(data, 1);
  //VCOM Control 1
  data[0] = 0x3E;
  data[1] = 0x28;
  TFT9341_SendCommand(0xC5);
  TFT9341_WriteData(data, 2);
  //VCOM Control 2
  data[0] = 0x86;
  TFT9341_SendCommand(0xC7);
  TFT9341_WriteData(data, 1);
  //Memory Acsess Control
  data[0] = 0x48;
  TFT9341_SendCommand(0x36);
  TFT9341_WriteData(data, 1);
  //Pixel Format Set
  data[0] = 0x55;//16bit
  TFT9341_SendCommand(0x3A);
  TFT9341_WriteData(data, 1);
  //Frame Rratio Control, Standard RGB Color
  data[0] = 0x00;
  data[1] = 0x18;
  TFT9341_SendCommand(0xB1);
  TFT9341_WriteData(data, 2);
  //Display Function Control
  data[0] = 0x08;
  data[1] = 0x82;
  data[2] = 0x27;//320 строк
  TFT9341_SendCommand(0xB6);
  TFT9341_WriteData(data, 3);
  //Enable 3G (пока не знаю что это за режим)
  data[0] = 0x00;//не включаем
  TFT9341_SendCommand(0xF2);
  TFT9341_WriteData(data, 1);
  //Gamma set
  data[0] = 0x01;//Gamma Curve (G2.2) (Кривая цветовой гаммы)
  TFT9341_SendCommand(0x26);
  TFT9341_WriteData(data, 1);
  //Positive Gamma  Correction
  data[0] = 0x0F;
  data[1] = 0x31;
  data[2] = 0x2B;
  data[3] = 0x0C;
  data[4] = 0x0E;
  data[5] = 0x08;
  data[6] = 0x4E;
  data[7] = 0xF1;
  data[8] = 0x37;
  data[9] = 0x07;
  data[10] = 0x10;
  data[11] = 0x03;
  data[12] = 0x0E;
  data[13] = 0x09;
  data[14] = 0x00;
  TFT9341_SendCommand(0xE0);
  TFT9341_WriteData(data, 15);
  //Negative Gamma  Correction
  data[0] = 0x00;
  data[1] = 0x0E;
  data[2] = 0x14;
  data[3] = 0x03;
  data[4] = 0x11;
  data[5] = 0x07;
  data[6] = 0x31;
  data[7] = 0xC1;
  data[8] = 0x48;
  data[9] = 0x08;
  data[10] = 0x0F;
  data[11] = 0x0C;
  data[12] = 0x31;
  data[13] = 0x36;
  data[14] = 0x0F;
  TFT9341_SendCommand(0xE1);
  TFT9341_WriteData(data, 15);
  TFT9341_SendCommand(0x11);//Выйдем из спящего режима
  HAL_Delay(120);
  //Display ON
  data[0] = TFT9341_ROTATION;
  TFT9341_SendCommand(0x29);
  TFT9341_WriteData(data, 1);
  TFT9341_WIDTH = w_size;
  TFT9341_HEIGHT = h_size;
}
//-------------------------------------------------------------------
void TFT9341_reset(void)
{
	RESET_ACTIVE();
	HAL_Delay(5);
	RESET_IDLE();
}
//-------------------------------------------------------------------
void TFT9341_SendCommand(uint8_t cmd)
{
  DC_COMMAND();
  HAL_SPI_Transmit(&hspi1, &cmd, 1, 5000);
}
//-------------------------------------------------------------------
void TFT9341_SendData(uint8_t dt)
{
	DC_DATA();
	HAL_SPI_Transmit(&hspi1, &dt, 1, 5000);
}
//-------------------------------------------------------------------
void TFT9341_WriteData(uint8_t* buff, size_t buff_size) {
	DC_DATA();
	while(buff_size > 0) {
		uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
		HAL_SPI_Transmit(&hspi1, buff, chunk_size, HAL_MAX_DELAY);
		buff += chunk_size;
		buff_size -= chunk_size;
	}
}
//-------------------------------------------------------------------
static void TFT9341_SetAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  // column address set
  TFT9341_SendCommand(0x2A); // CASET
  {
    uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
    TFT9341_WriteData(data, sizeof(data));
  }

  // row address set
  TFT9341_SendCommand(0x2B); // RASET
  {
    uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
    TFT9341_WriteData(data, sizeof(data));
  }

  // write to RAM
  TFT9341_SendCommand(0x2C); // RAMWR
}//-------------------------------------------------------------------
void TFT9341_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  if((x1 >= TFT9341_WIDTH) || (y1 >= TFT9341_HEIGHT) || (x2 >= TFT9341_WIDTH) || (y2 >= TFT9341_HEIGHT)) return;
	if(x1>x2) swap(x1,x2);
	if(y1>y2) swap(y1,y2);
  TFT9341_SetAddrWindow(x1, y1, x2, y2);
  uint8_t data[] = { color >> 8, color & 0xFF };
  DC_DATA();
  for(uint32_t i = 0; i < (x2-x1+1)*(y2-y1+1); i++)
  {
      HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
  }
}
//-------------------------------------------------------------------
void TFT9341_FillScreen(uint16_t color)
{
  TFT9341_FillRect(0, 0, TFT9341_WIDTH-1, TFT9341_HEIGHT-1, color);
}
//-------------------------------------------------------------------

